#ifndef DXSYSCLASS_H
#define DXSYSCLASS_H

//减少api的使用使win32的头文件变小，加快构建过程
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>

#include"DxInputClass.h"
#include"DxGraphicsClass.h"
#include"DxTimerClass.h"
#include"DxCpuClass.h";
#include"DxFpsClass.h"

class DxSysClass
{
public:
	DxSysClass();
	DxSysClass(const DxSysClass& other);
	~DxSysClass();

	bool Init();
	void ShutDown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	/*
	LRESULT//表示会返回多种long型值
	CALLBACK//只是为了识别这是一个回调函数的空宏
	HWND//窗口句柄，整型值
	UINT//unsigned int,WM_NOTIFY
	WPARAM//typedef UINT WPARAM;control identifier
	LPARAM//typedef LONG LPARAM;notification messages
	*/
private:
	bool Frame();
	void WindowsInit(int& screenWidth, int& screenHeight);
	void WindowsShutDown();

private:
	/*
	LPCTSTR用来表示你的字符是否使用UNICODE,
	如果你的程序定义了UNICODE或者其他相关的宏，
	那么这个字符或者字符串将被作为UNICODE字符串，
	否则就是标准的ANSI字符串
	*/
	LPCWSTR m_windowsName;
	HINSTANCE m_instance;
	HWND m_hwnd;// The hwnd variable is a handle to the window

	DxInputClass* m_input;
	DxGraphicsClass* m_graph;

	DxFpsClass* m_fps;
	DxCpuClass* m_cpuUsage;
	DxTimerClass* m_Timer;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static DxSysClass* ApplicationHandle = 0;



#endif // DXSYSCLASS_H
