#ifndef DX3DCLASS_H
#define DX3DCLASS_H

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <Windows.h>
#include <D3D11.h>
#include <DirectXMath.h>
#include <stdlib.h>
using namespace DirectX;

const float PI = 3.141592654f;

class Dx3dClass
{
public:
	Dx3dClass();
	Dx3dClass(const Dx3dClass& other);
	~Dx3dClass();

	bool Init(int screenWidth, int screenHeight, bool vsync,HWND hwnd, bool fullScreen, float screenDepth, float screenNear);
	void ShutDown();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice();//设备
	ID3D11DeviceContext* GetDeviceContext();//渲染上下文

	void GetProjectionMatrix(XMMATRIX& proMatrix);
	void GetWorldMatrix(XMMATRIX& worldMatrix);
	void GetOrthoMatrix(XMMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* name, int&);
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif// DX3DCLASS_H
