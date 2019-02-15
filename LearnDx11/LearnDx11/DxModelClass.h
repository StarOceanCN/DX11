#ifndef DXMODELCLASS_H
#define DXMODELCLASS_H

#include<d3d11.h>
#include<D3DX10math.h>

#include"DxTextureClass.h"
#include <fstream>
using namespace std;

//3d模型载入模块，顶点数据为坐标、UV、法线。
class DxModelClass
{
public:
	DxModelClass();
	DxModelClass(const DxModelClass& other);
	~DxModelClass();
	bool Init(ID3D11Device* device, const char* modelFileName, const WCHAR* textureFileName);
	void ShutDown();
	void Render(ID3D11DeviceContext* deviceContext);
	ID3D11ShaderResourceView* GetTexture();

	int GetIndexCount();

private:
	bool BufferInit(ID3D11Device* device);
	void BufferShutDown();
	void BufferRender(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, const WCHAR* fileName);
	void ReleaseTexture();

	bool LoadModel(const char* modelFileName);
	void ReleaseModel();


private:
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
	
	DxTextureClass* m_texture;
	ModelType* m_model;
};


#endif