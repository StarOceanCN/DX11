#ifndef DXGRAPHICSCLASS_H
#define DXGRAPHICSCLASS_H

#include"Dx3dClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class DxGraphicsClass
{
public:
	DxGraphicsClass();
	DxGraphicsClass(const DxGraphicsClass &other);
	~DxGraphicsClass();
	bool Init(int screenWidth, int screenHeight, HWND m_hwnd);
	void ShutDown();
	bool Frame();

private:
	bool Render();

private:
	Dx3dClass* m_dx3dcls;
};


#endif //DXGTAPHICSCLASS_H