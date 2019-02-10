#ifndef DXSHADERCLASS_H
#define DXSHADERCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

class DxShaderClass
{
public:
	DxShaderClass();
	DxShaderClass(const DxShaderClass& other);
	~DxShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd, const WCHAR* vertexShader,const WCHAR* pixelShader);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMartix, ID3D11ShaderResourceView* texture, 
		D3DXVECTOR3 lightDirection , D3DXVECTOR4 diffuseColor);

private:
	bool ShaderInit(ID3D11Device* devic, HWND hwnd, const WCHAR* vertexShader, const WCHAR* pixelShader);
	void ShaderShutDown();
	void ShaderOutputErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* fileName);

	bool ShaderSetParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, 
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor);
	void ShaderRender(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	struct MatrixBufferType {
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
	};

	struct LightBufferType {
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

#endif // !DXSHADERCLASS_H
