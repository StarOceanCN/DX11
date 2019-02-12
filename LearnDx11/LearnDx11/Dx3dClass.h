#ifndef DX3DCLASS_H
#define DX3DCLASS_H

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>

#include <D3DX10math.h>


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

	void GetProjectionMatrix(D3DXMATRIX& proMatrix);
	void GetWorldMatrix(D3DXMATRIX& worldMatrix);
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* name, int&);

	void ZBufferTurnOn();
	void ZBufferTurnOff();

	void CullingTurnOn();
	void CullingTurnOff();

	void AlphaBlendingTurnOn();
	void AlphaBlendingTurnOff();

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
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif// DX3DCLASS_H
