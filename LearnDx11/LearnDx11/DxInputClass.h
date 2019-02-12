#ifndef DXINPUTCLASS_H
#define DXINPUTCLASS_H
//You need to define the version of Direct Input you are using in the header or 
//the compiler will generate annoying messages that it is defaulting to version 8.
#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include<dinput.h>

class DxInputClass
{
public:
	DxInputClass();
	DxInputClass(const DxInputClass& other);
	~DxInputClass();

	bool Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void ShutDown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& x, int& y);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};


#endif //DXINPUTCLASS_H
