#ifndef DXSYSCLASS_H
#define DXSYSCLASS_H

//����api��ʹ��ʹwin32��ͷ�ļ���С���ӿ칹������
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
	LRESULT//��ʾ�᷵�ض���long��ֵ
	CALLBACK//ֻ��Ϊ��ʶ������һ���ص������Ŀպ�
	HWND//���ھ��������ֵ
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
	LPCTSTR������ʾ����ַ��Ƿ�ʹ��UNICODE,
	�����ĳ�������UNICODE����������صĺ꣬
	��ô����ַ������ַ���������ΪUNICODE�ַ�����
	������Ǳ�׼��ANSI�ַ���
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
