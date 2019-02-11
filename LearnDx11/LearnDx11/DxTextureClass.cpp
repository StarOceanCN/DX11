#include"DxTextureClass.h"

DxTextureClass::DxTextureClass(){
	m_texture = 0;
}

DxTextureClass::DxTextureClass(const DxTextureClass& other) {}
DxTextureClass::~DxTextureClass() {}

bool DxTextureClass::Init(ID3D11Device* device, const WCHAR* fileName) {
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_texture, NULL);

	if (FAILED(result)) {
		return false;
	}

	return true;
}

void DxTextureClass::ShutDown() {
	if (m_texture) {
		m_texture->Release();
		m_texture = 0;
	}
}

ID3D11ShaderResourceView* DxTextureClass::GetTexture() {
	return m_texture;
}
