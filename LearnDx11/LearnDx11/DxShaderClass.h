#ifndef DXSHADERCLASS_H
#define DXSHADERCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;
//shader读入模块，包括带有光线属性的shader读入和普通的纹理shader读入
//重写继承化
class DxShaderClass
{
public:
	DxShaderClass();
	DxShaderClass(const DxShaderClass& other);
	~DxShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd, const WCHAR* vertexShader,const WCHAR* pixelShader, const char* VSname, const char* PSname);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMartix, ID3D11ShaderResourceView* texture, 
		D3DXVECTOR3 lightDirection , D3DXVECTOR4 diffuseColor, D3DXVECTOR4 ambientColor);

private:
	bool ShaderInit(ID3D11Device* devic, HWND hwnd, const WCHAR* vertexShader, const WCHAR* pixelShader, const char* VSname, const char* PSname);
	void ShaderShutDown();
	void ShaderOutputErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* fileName);

	bool ShaderSetParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, 
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor, D3DXVECTOR4 ambientColor);
	void ShaderRender(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	struct MatrixBufferType {
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
	};

	struct LightBufferType {
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuesColor;
		D3DXVECTOR3 lightDirection;
		float padding;// Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
};

class DxTextureShaderClass
{
public:
	DxTextureShaderClass();
	DxTextureShaderClass(const DxTextureShaderClass& other);
	~DxTextureShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd, const WCHAR* vertexShader, const WCHAR* pixelShader, const char* VSname, const char* PSname);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMartix, ID3D11ShaderResourceView* texture , D3DXVECTOR4* pixelColor);

private:
	bool ShaderInit(ID3D11Device* devic, HWND hwnd, const WCHAR* vertexShader, const WCHAR* pixelShader, const char* VSname, const char* PSname);
	void ShaderShutDown();
	void ShaderOutputErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* fileName);

	bool ShaderSetParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4* pixelColor);
	void ShaderRender(ID3D11DeviceContext* deviceContext, int indexCount);
	bool ShaderSetPixelBuffer();

private:
	struct MatrixBufferType {
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
	};

	struct PixelBufferType
	{
		D3DXVECTOR4 pixelColor;
	};

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_pixelBuffer;
};

#endif // !DXSHADERCLASS_H
