#include "DxSysClass.h"

DxSysClass::DxSysClass() {
	m_input = 0;
	m_graph = 0;
}

DxSysClass::DxSysClass(const DxSysClass& other) {

}

DxSysClass::~DxSysClass() {

}

bool DxSysClass::Init() {
	int screenWidth = 0, 
		screenHeight = 0;
	bool isSuccess;

	WindowsInit(screenWidth, screenHeight);
	m_input = new DxInputClass();
	if (!m_input) {
		return false;
	}

	isSuccess = m_input->Init(m_instance, m_hwnd, screenWidth, screenHeight);
	if (!isSuccess) {
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}
	
	m_graph = new DxGraphicsClass();
	if (!m_graph) {
		return false;
	}
	isSuccess = m_graph->Init(screenWidth, screenHeight, m_hwnd);
	return isSuccess;
}

void DxSysClass::ShutDown() {
	if (m_input) {
		m_input->ShutDown();
		delete m_input;
		m_input = 0;
	}

	if (m_graph) {
		m_graph->ShutDown();
		delete m_graph;
		m_graph = 0;
	}

	WindowsShutDown();
	
}

void DxSysClass::Run() {
	MSG msg;
	bool done, isSuccess;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			done = true;
		else {
			isSuccess = Frame();
			if (!isSuccess) {
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}
		if (m_input->IsEscapePressed())
			done = true;

	}
}

bool DxSysClass::Frame() {
	bool isSuccess;
	int mouseX, mouseY;

	// Do the input frame processing.
	isSuccess = m_input->Frame();
	if (!isSuccess){
		return false;
	}
	// Get the location of the mouse from the input object,
	m_input->GetMouseLocation(mouseX, mouseY);

	isSuccess = m_graph->Frame(mouseX, mouseY);
	if (!isSuccess)
		return false;

	return true;
}

LRESULT CALLBACK DxSysClass::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//remove 
	/*
	switch (msg)
	{
	case WM_KEYDOWN:
		m_input->KeyDown((unsigned int)wparam);
		return 0;
	case WM_KEYUP:
		m_input->KeyUp((unsigned int)wparam);
		return 0;
	default:*/
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void DxSysClass::WindowsInit(int& screenWidth, int& screenHeight) {
	WNDCLASSEX win;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;
	m_instance = GetModuleHandle(NULL);

	m_windowsName = L"Dx11 Render";

	win.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	win.lpfnWndProc = WndProc;
	win.cbClsExtra = 0;
	win.cbWndExtra = 0;
	win.hInstance = m_instance;
	win.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	win.hIconSm = win.hIcon;
	win.hCursor = LoadCursor(NULL, IDC_ARROW);
	win.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	win.lpszMenuName = NULL;
	win.lpszClassName = m_windowsName;
	win.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&win);

	//获取主屏幕高度和宽度
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) {
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		// dmpelswidth dmpelsheight dmbitsperpel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else {
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_windowsName, m_windowsName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_instance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);
}

void DxSysClass::WindowsShutDown() {
	ShowCursor(true);

	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_windowsName, m_instance);

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return ApplicationHandle->MessageHandler(hwnd, msg, wparam, lparam);
	}
}

