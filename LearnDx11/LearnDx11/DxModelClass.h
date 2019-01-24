#ifndef DXMODELCLASS_H
#define DXMODELCLASS_H

#include<d3d11.h>
#include<D3DX10math.h>


class DxModelClass
{
public:
	DxModelClass();
	DxModelClass(const DxModelClass& other);
	~DxModelClass();
	bool Init(ID3D11Device* device);
	void ShutDown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	bool BufferInit(ID3D11Device* device);
	void BufferShutDown();
	void BufferRender(ID3D11DeviceContext* deviceContext);

private:
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;

};


#endif