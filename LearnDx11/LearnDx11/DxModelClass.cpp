#include "DxModelClass.h"

DxModelClass::DxModelClass() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}
DxModelClass::DxModelClass(const DxModelClass& other) {

}

DxModelClass::~DxModelClass() {}

bool DxModelClass::Init(ID3D11Device* device, const WCHAR* fileName) {
	bool isSuccess;
	isSuccess = BufferInit(device);
	if (!isSuccess)
		return false;

	isSuccess = LoadTexture(device, fileName);
	if (!isSuccess) {
		return false;
	}

	return true;
}

void DxModelClass::ShutDown() {
	ReleaseTexture();

	BufferShutDown();
}

void DxModelClass::Render(ID3D11DeviceContext* deviceContext) {
	//put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	BufferRender(deviceContext);
}

int DxModelClass::GetIndexCount() {
	return m_indexCount;
}

ID3D11ShaderResourceView* DxModelClass::GetTexture() {
	return m_texture->GetTexture();
}

bool DxModelClass::BufferInit(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//set the number of vertices in the vertex array
	m_vertexCount = 3;

	//set the number of indices in the index array
	m_indexCount = 3;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];

	if (!indices)
		return false;

	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);//bottom left
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//top middle
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);//bottom right
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	//set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	//set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;
	
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool DxModelClass::LoadTexture(ID3D11Device* device, const WCHAR* fileName) {
	bool isSuccess;
	m_texture = new DxTextureClass();
	if (!m_texture) {
		return false;
	}

	isSuccess = m_texture->Init(device, fileName);
	if (!isSuccess) {
		return false;
	}

	return true;
}

void DxModelClass::ReleaseTexture() {
	if (m_texture) {
		m_texture->ShutDown();
		m_texture = 0;
	}
}

void DxModelClass::BufferShutDown() {
	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void DxModelClass::BufferRender(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;//²½³¤
	unsigned int offset;//Æ«ÒÆ
	
	stride = sizeof(VertexType);
	offset = 0;

	//set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set the type of primitive that should be rendered from this vertex buffer ,inthis case triangle
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
