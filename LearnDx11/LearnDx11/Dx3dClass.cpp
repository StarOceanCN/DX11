#include "Dx3dClass.h"

Dx3dClass::Dx3dClass() {
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

Dx3dClass::Dx3dClass(const Dx3dClass& other) {

}

Dx3dClass::~Dx3dClass() {

}
//The Initialize function is what does the entire setup of Direct3D for DirectX 11.
bool Dx3dClass::Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear) {
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned int stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	m_vsync_enabled = vsync;


	/*
	Before we can initialize Direct3D we have to get the refresh rate from the video card/monitor.
	Each computer may be slightly different so we will need to query for that information.
	We query for the numerator and denominator values and then pass them to DirectX during the setup and it will calculate the proper refresh rate.
	If we don't do this and just set the refresh rate to a default value which may not exist on all computers then DirectX will respond by 
	performing a blit instead of a buffer flip which will degrade performance and give us annoying errors in the debug output.
	适配硬件
	*/
	//Create a Dx Graphics interface factory
	bool isSuccess = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);// todo
	if (FAILED(isSuccess)) {
		return false;
	}

	//Use the factory to create an adapter for the primary graphics interface(video card)
	isSuccess = factory->EnumAdapters(0, &adapter);
	if (FAILED(isSuccess))
		return false;

	//Enumerate the primaryadapter output(monitor) 
	isSuccess = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(isSuccess))
		return false;

	//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display foamat for the adapter output(monitor)
	isSuccess = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(isSuccess))
		return false;

	//Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	//now fill the display mode list structures
	isSuccess = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (isSuccess) {
		return false;
	}

	//now go through all the display modes and find the one that matches the screen width and height
	//when a match is found store the numerator and denominator of the refresh rate for that monitor
	for (int i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == (unsigned int)screenWidth) {
			if (displayModeList[i].Height == (unsigned int)screenHeight) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//Get the adapter(video card) descripthon.
	isSuccess = adapter->GetDesc(&adapterDesc);
	if (FAILED(isSuccess))
		return false;

	//store the dedicated video card memory in megabyteds
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//convert the name of the video card too a character array and store it.
	//宽字符转多字符
	error = wcstombs_s(&stringLength, m_videoCardDescription, (size_t)128, adapterDesc.Description, (size_t)128);
	if (error != 0)
		return false;

	//release the display mode list
	delete[] displayModeList;
	displayModeList = 0;

	//release the adapter ouyput
	adapterOutput->Release();
	adapterOutput = 0;

	//release the adapter
	adapter->Release();
	adapter = 0;

	//release the factory
	factory->Release();
	factory = 0;

	//Initialize the swap chain description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	/*
	1．BufferDesc：该结构体描述了我们所要创建的后台缓冲区的属性。我们主要关注的属性有：宽度、高度和像素格式
	2．SampleDesc：多重采样数量和质量级别
	3．BufferUsage：设为DXGI_USAGE_RENDER_TARGET_OUTPUT，因为我们要将场景渲染到后台缓冲区（即，将它用作渲染目标）。
	4．BufferCount：交换链中的后台缓冲区数量；我们一般只用一个后台缓冲区来实现双缓存。当然，你也可以使用两个后台缓冲区来实现三缓存。
	5．OutputWindow：我们将要渲染到的窗口的句柄。
	6．Windowed：当设为true时，程序以窗口模式运行；当设为false时，程序以全屏（full-screen）模式运行。
	7．SwapEffect：设为DXGI_SWAP_EFFECT_DISCARD，让显卡驱动程序选择最高效的显示模式。
	8．Flags：可选的标志值。如果设为DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH，那么当应用程序切换到全屏模式时，
	Direct3D会自动选择与当前的后台缓冲区设置最匹配的显示模式。如果未指定该标志值，那么当应用程序切换到全屏模式时，
	Direct3D会使用当前的桌面显示模式。
	*/
	//set to a single back buffer
	swapChainDesc.BufferCount = 1;

	//set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//set regular 32-bit surface for the back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// The refresh rate is how many times a second it draws the back buffer to the front buffer. 
	//set the refresh rate of the back buffer
	if (m_vsync_enabled) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//set the handle for the window to render to
	swapChainDesc.OutputWindow = hwnd;

	//trun multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//set to full screen of windowed mode
	if (fullScreen) {
		swapChainDesc.Windowed = false;

	}
	else {
		swapChainDesc.Windowed = true;
	}

	//set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//dont set the advanced flags
	swapChainDesc.Flags = 0;

	//set the feature level to dx11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	isSuccess = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	if (FAILED(isSuccess)) {
		return false;
	}

	//get the pointer to the back buffer
	isSuccess = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(isSuccess)) {
		return false;
	}

	//create the render target view with the back buffer poniter
	isSuccess = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(isSuccess)) {
		return false;
	}

	//release pointer to the back buffer as we no longer need it
	backBufferPtr->Release();
	backBufferPtr = 0;

	//initialize the description of the depth buffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//set pu the description of the depth buffer
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//create the texture for the depth buffer using the filled out description
	isSuccess = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(isSuccess)) {
		return false;
	}

	//initialize the description of the stencil state
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//set up the description of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//stencil opeerations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create the depth stencil state
	isSuccess = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(isSuccess))
		return false;

	//set the depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//create the depth stencil view
	isSuccess = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(isSuccess))
		return false;

	//bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//create the rasterizer state from the description we just filled out
	isSuccess = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(isSuccess)) {
		return false;
	}

	//now set the rasterizer state
	m_deviceContext->RSSetState(m_rasterState);

	//setup the viewport for rendering 
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//create the view port
	m_deviceContext->RSSetViewports(1, &viewport);

	//setup the projection matrix
	fieldOfView = PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	//create the projection matrix for 3d rendering
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	//initialize the world matrix to the identity matrix
	m_worldMatrix = XMMatrixIdentity();

	//create an orthographic projection matrix for 2d rendering
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void Dx3dClass::ShutDown() {
	if (m_swapChain) {
		m_swapChain->SetFullscreenState(false, NULL);
	}
	if (m_rasterState) {
		m_rasterState->Release();
		m_rasterState = 0;
	}
	if (m_depthStencilView) {
		m_depthStencilView->Release();
		m_depthStencilView = 0;

	}

	if (m_depthStencilBuffer) {
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
	if (m_renderTargetView) {
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext) {
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device) {
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain) {
		m_swapChain->Release();
		m_swapChain = 0;
	}

}
void Dx3dClass::BeginScene(float r, float g, float b, float a) {
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void Dx3dClass::EndScene() {
	if (m_vsync_enabled) {
		m_swapChain->Present(1, 0);
	}
	else {
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* Dx3dClass::GetDevice() {
	return m_device;
}

ID3D11DeviceContext* Dx3dClass::GetDeviceContext() {
	return m_deviceContext;
}

void Dx3dClass::GetProjectionMatrix(XMMATRIX& proMatix) {
	proMatix = m_projectionMatrix;
}
void Dx3dClass::GetWorldMatrix(XMMATRIX& worldMatrix) {
	worldMatrix = m_worldMatrix;
}
void Dx3dClass::GetOrthoMatrix(XMMATRIX& orthoMatrix) {
	orthoMatrix = m_orthoMatrix;
}
void Dx3dClass::GetVideoCardInfo(char* cardName, int& memory) {
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}


