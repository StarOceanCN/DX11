#include "DxModelClass.h"

DxModelClass::DxModelClass() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_model = 0;
}
DxModelClass::DxModelClass(const DxModelClass& other) {

}

DxModelClass::~DxModelClass() {}

bool DxModelClass::Init(ID3D11Device* device, const char* modelFileName, const WCHAR* textureFileName) {
	bool isSuccess;
	//载入模型
	isSuccess = LoadModel(modelFileName);
	if (!isSuccess)
	{
		return false;
	}
	//读取纹理
	isSuccess = LoadTexture(device, textureFileName);
	if (!isSuccess) {
		return false;
	}
	//缓冲区初始化
	isSuccess = BufferInit(device);
	if (!isSuccess)
		return false;

	return true;
}

void DxModelClass::ShutDown() {
	ReleaseTexture();

	BufferShutDown();

	ReleaseModel();
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
	//m_vertexCount = 3;

	//set the number of indices in the index array
	//m_indexCount = 3;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];

	if (!indices)
		return false;

	// Load the vertex array and index array with data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}
/*
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);//bottom left
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//top middle
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);//bottom right
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
*/
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
	unsigned int stride;//步长
	unsigned int offset;//偏移
	
	stride = sizeof(VertexType);
	offset = 0;

	//set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set the type of primitive that should be rendered from this vertex buffer ,inthis case triangle
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
//自定义顶点格式，包括顶点数和绘制顶点顺序
bool DxModelClass::LoadModel(const char* modelFileName)
{
	ifstream fin;
	char input;
	int i;

	fin.open(modelFileName);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void DxModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}
