#include"Dx2DRenderClass.h"

Dx2DRenderClass::Dx2DRenderClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
}


Dx2DRenderClass::Dx2DRenderClass(const Dx2DRenderClass& other){
}


Dx2DRenderClass::~Dx2DRenderClass(){
}

//初始化
bool Dx2DRenderClass::Init(ID3D11Device* device, int screenWidth, int screenHeight, const WCHAR* textureFileName, int bitmapWidth, int bitmapHeight){
	bool isSuccess;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

	isSuccess = BufferInit(device);
	if (!isSuccess)
	{
		return false;
	}

	isSuccess = LoadTexture(device, textureFileName);
	if (!isSuccess)
	{
		return false;
	}

	return true;
}

void Dx2DRenderClass::Shutdown(){
	ReleaseTexture();

	BufferShutDown();

	return;
}

bool Dx2DRenderClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY){
	bool isSuccess;

	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	isSuccess = BufferUpdate(deviceContext, positionX, positionY);
	if (!isSuccess)
	{
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	BufferRender(deviceContext);

	return true;
}

int Dx2DRenderClass::GetIndexCount(){
	return m_indexCount;
}

ID3D11ShaderResourceView* Dx2DRenderClass::GetTexture(){
	return m_texture->GetTexture();
}

//初始化缓冲区
bool Dx2DRenderClass::BufferInit(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	//填充顶点缓冲描述结构体
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//填充定点索引缓冲区描述结构体
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Dx2DRenderClass::BufferShutDown()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
//更新定点缓冲区内容，用于适配2dui改变位置
bool Dx2DRenderClass::BufferUpdate(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	//如果位置不变就停止更新
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	m_previousPosX = positionX;
	m_previousPosY = positionY;

	//计算ui左端坐标
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

	//右端坐标
	right = left + (float)m_bitmapWidth;

	//上端坐标
	top = (float)(m_screenHeight / 2) - (float)positionY;

	//底部坐标
	bottom = top - (float)m_bitmapHeight;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	//锁定缓冲区，写入
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//获取定点缓冲区指针
	verticesPtr = (VertexType*)mappedResource.pData;

	//数据写入缓冲区
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	//解锁
	deviceContext->Unmap(m_vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

void Dx2DRenderClass::BufferRender(ID3D11DeviceContext* deviceContext){
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);//步长
	offset = 0;//偏移

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

bool Dx2DRenderClass::LoadTexture(ID3D11Device* device, const WCHAR* filename){
	bool isSuccess;

	m_texture = new DxTextureClass;
	if (!m_texture)
	{
		return false;
	}

	// Initialize the texture object.
	isSuccess = m_texture->Init(device, filename);
	if (!isSuccess)
	{
		return false;
	}

	return true;
}

void Dx2DRenderClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_texture)
	{
		m_texture->ShutDown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}