#include "DxSysClass.h"

DxSysClass::DxSysClass() {
	m_input = 0;
	m_graph = 0;
	m_fps = 0;
	m_cpuUsage = 0;
	m_Timer = 0;
	m_move = 0;
	isFirst = false;
}

DxSysClass::DxSysClass(const DxSysClass& other) {

}

DxSysClass::~DxSysClass() {

}

bool DxSysClass::Init() {
	int screenWidth = 0, 
		screenHeight = 0;
	bool isSuccess;
	//windows初始化
	WindowsInit(screenWidth, screenHeight);
	//输入模块
	m_input = new DxInputClass();
	if (!m_input) {
		return false;
	}

	isSuccess = m_input->Init(m_instance, m_hwnd, screenWidth, screenHeight);
	if (!isSuccess) {
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}
	//渲染模块
	m_graph = new DxGraphicsClass();
	if (!m_graph) {
		return false;
	}
	isSuccess = m_graph->Init(screenWidth, screenHeight, m_hwnd);

	//fps计算模块
	m_fps = new DxFpsClass();
	if (!m_fps){
		return false;
	}
	m_fps->Init();

	//cpu使用率模块
	m_cpuUsage = new DxCpuClass();
	if (!m_cpuUsage){
		return false;
	}
	m_cpuUsage->Init();

	//定时器模块
	m_Timer = new DxTimerClass();
	if (!m_Timer){
		return false;
	}
	isSuccess = m_Timer->Init();
	if (!isSuccess){
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_move = new DxMoveClass();
	if (!m_move) {
		return false;
	}


	return true;
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
	if (m_cpuUsage) {
		m_cpuUsage->ShutDown();
		delete m_cpuUsage;
		m_cpuUsage = 0;
	}
	if (m_fps) {
		delete m_fps;
		m_fps = 0;
	}

	if (m_Timer) {
		delete m_Timer;
		m_Timer = 0;
	}

	if (m_move) {
		delete m_move;
		m_move = 0;
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
		if (m_input->IsKeyPressed(DIK_ESCAPE))
			done = true;

	}
}

bool DxSysClass::Frame() {
	bool isSuccess;
	int mouseX, mouseY;

	//计算fps
	m_fps->Frame();
	m_Timer->Frame();
	//计算cpu使用
	m_cpuUsage->Frame();

	//输入获取
	isSuccess = m_input->Frame();
	if (!isSuccess){
		return false;
	}
	//获取鼠标位置
	m_input->GetMouseLocation(mouseX, mouseY);
	//按键响应
	m_move->SetFrameTime(m_Timer->GetTime());
	m_move->MoveForward(m_input->IsKeyPressed(DIK_W));
	m_move->MoveBackward(m_input->IsKeyPressed(DIK_S));
	m_move->TurnLeft(m_input->IsKeyPressed(DIK_A));
	m_move->TurnRight(m_input->IsKeyPressed(DIK_D));
	//第一人称/第三人称
	if (m_input->IsKeyPressed(DIK_1)) {
		isFirst = true;
	}
	if (m_input->IsKeyPressed(DIK_2)) {
		isFirst = false;
	}

	isSuccess = m_graph->Frame(mouseX, mouseY, m_fps->GetFps(), m_cpuUsage->GetCpuPercentage(), m_move, isFirst);
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

