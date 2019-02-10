#ifndef DX2DRENDERCLASS_H
#define DX2DRENDERCLASS_H

#include <d3d11.h>
#include <d3dx10math.h>
#include"DxTextureClass.h"

class Dx2DRenderClass
{
private:
	struct VertexType{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Dx2DRenderClass();
	Dx2DRenderClass(const Dx2DRenderClass& other);
	~Dx2DRenderClass();

	bool Init(ID3D11Device* device, int screenWidth, int screenHeight, const WCHAR* textureFileName, int bitmapWidth, int bitmapHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool BufferInit(ID3D11Device* device);
	void BufferShutDown();
	bool BufferUpdate(ID3D11DeviceContext* deviceContext, int, int);
	void BufferRender(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device,const WCHAR* textureFileName);
	void ReleaseTexture();
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	DxTextureClass* m_texture;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif //DX2DRENDERCLASS_H