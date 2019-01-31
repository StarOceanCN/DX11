#ifndef DXTEXTURECLASS_H
#define DXTEXTURECLASS_H

#include<D3D11.h>
#include<D3DX11tex.h>

class DxTextureClass
{
public:
	DxTextureClass();
	DxTextureClass(const DxTextureClass& other);
	~DxTextureClass();

	bool Init(ID3D11Device* device, const WCHAR* fileName);
	void ShutDown();

	ID3D11ShaderResourceView* GetTexture();

private:

	ID3D11ShaderResourceView* m_texture;
};


#endif // !DXTEXTURECLASS_H

