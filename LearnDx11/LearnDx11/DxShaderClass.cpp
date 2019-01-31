#include "DxShaderClass.h"

DxShaderClass::DxShaderClass() {
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}
DxShaderClass::DxShaderClass(const DxShaderClass& other) {

}

DxShaderClass::~DxShaderClass() {}

bool DxShaderClass::Init(ID3D11Device* device, HWND hwnd, const WCHAR* vertexShader, const WCHAR* pixelShader) {
	bool isSuccess;
	isSuccess = ShaderInit(device, hwnd, vertexShader, pixelShader);
	if (!isSuccess)
		return false;
	return true;
}

void DxShaderClass::ShutDown() {
	ShaderShutDown();
}

bool DxShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) {
	bool isSuccess;

	//set the shader parameters
	isSuccess = ShaderSetParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!isSuccess)
		return false;

	ShaderRender(deviceContext, indexCount);

	return true;
}

/*
* about ID3D10Blob
* Blobs can be used as a data buffer, storing vertex, adjacency, and material information during mesh optimization and loading operations. 
* Also, these objects are used to return object code and error messages in APIs that compile vertex, geometry and pixel shaders.
*/
bool DxShaderClass::ShaderInit(ID3D11Device* device, HWND hwnd, const WCHAR* vertexShader, const WCHAR* pixelShader) {
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC sampleDesc;

	// initialize the pointer this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;


	result = D3DX11CompileFromFile(vertexShader, NULL, NULL, "TextureVertexShader", "vs_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);


	if (FAILED(result)) {
		if (errorMessage) {
			ShaderOutputErrorMessage(errorMessage, hwnd, vertexShader);
		}
		else {
			MessageBox(hwnd, vertexShader, L"Cant find vertex shader file!", MB_OK);
		}
		return false;
	}

	result = D3DX11CompileFromFile(pixelShader, NULL, NULL, "TexturePixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result)) {
		if (errorMessage) {
			ShaderOutputErrorMessage(errorMessage, hwnd, pixelShader);
		}
		else {
			MessageBox(hwnd, pixelShader, L"Cant find pixel shader file!", MB_OK);
		}
		return false;

	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result)) {
		return false;
	}
	
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);

	if (FAILED(result)) {
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;//it will figure out the spacing for you
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), &m_layout);

	if (FAILED(result)) {
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//setup the description of the dynamic matrix constant buffer that is in the vertex shader;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) {
		return false;
	}

	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.MipLODBias = 0.0f;;
	sampleDesc.MaxAnisotropy = 1;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampleDesc.BorderColor[0] = 0;
	sampleDesc.BorderColor[1] = 0;
	sampleDesc.BorderColor[2] = 0;
	sampleDesc.BorderColor[2] = 0;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&sampleDesc, &m_sampleState);
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void DxShaderClass::ShaderShutDown(){

	if (m_sampleState) {
		m_sampleState->Release();
		m_sampleState = 0;
	}
	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	if (m_layout) {
		m_layout->Release();
		m_layout = 0;
	}
	if (m_pixelShader) {
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	if (m_vertexShader) {
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void DxShaderClass::ShaderOutputErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* fileName) {
	char* compileErrors;
	unsigned int bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader_error_log.txt");

	for (int i = 0; i < bufferSize; i++) {
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Shader compiling failed, check shader_error_log.txt for message", fileName, MB_OK);

}

bool DxShaderClass::ShaderSetParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) {

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->wordMatrix = worldMatrix;
	dataPtr->viewMatrix = viewMatrix;
	dataPtr->projectionMatrix = projectionMatrix;

	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	if (texture != NULL) {
		deviceContext->PSSetShaderResources(0, 1, &texture);
	}

	return true;
}

void DxShaderClass::ShaderRender(ID3D11DeviceContext* deviceContext, int indexCount) {
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	if (m_sampleState) {
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	}

	deviceContext->DrawIndexed(indexCount, 0, 0);
}

