#ifndef DXGRAPHICSCLASS_H
#define DXGRAPHICSCLASS_H

#include"Dx3dClass.h"
#include"DxCameraClass.h"
#include"DxModelClass.h"
#include"DxShaderClass.h"
#include"DxLightClass.h"

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
	bool Render(float rotation);

private:
	Dx3dClass* m_dx3dcls;
	DxCameraClass* m_camera;
	DxModelClass* m_model;
	DxShaderClass* m_shader;
	DxLightClass* m_light;
};


#endif //DXGTAPHICSCLASS_H