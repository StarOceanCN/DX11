#ifndef DXFONTCLASS_H
#define DXFONTCLASS_H

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;

#include"DxTextureClass.h"
/*
* 字形设置模块，对文件中字形的读取，ascii码解析
*/
class DxFontClass
{
public:
	DxFontClass();
	DxFontClass(const DxFontClass& other);
	~DxFontClass();

	bool Init(ID3D11Device* device, const char* fontFileName, const WCHAR* textureFileName);
	void ShutDown();
	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	struct FontType {
		float left, right;
		int size;
	};
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

private:
	bool LoadFontData(const char* fontFileName);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, const WCHAR* textureFineName);
	void ReleaseTexture();

private:
	FontType* m_font;
	DxTextureClass* m_texture;
};

#endif //DXFONTCLASS_H