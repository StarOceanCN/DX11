#ifndef DXGRAPHICSCLASS_H
#define DXGRAPHICSCLASS_H

#include"Dx3dClass.h"
#include"DxCameraClass.h"
#include"DxModelClass.h"
#include"DxShaderClass.h"
#include"DxLightClass.h"
#include"Dx2DRenderClass.h"
#include"DxTextClass.h"
#include"DxMoveClass.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
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
	bool Frame(int x, int y, int fps, int cpuUsage, DxMoveClass* movePosition, bool isFirst, int vehicleDir);

private:
	bool Render(DxMoveClass* movePosition, bool isFirst, int vehicleDir);

private:
	Dx3dClass* m_dx3dcls;
	DxCameraClass* m_camera;
	DxModelClass* m_skybox, *m_floor, *m_vehicleTop, *m_vehicleBottom, *m_vehicleTire;
	DxShaderClass* m_modelShader, *m_skyboxShader;
	DxTextureShaderClass *m_2dShader;
	DxLightClass* m_light;
	Dx2DRenderClass* m_bitmap;
	DxTextClass* m_text;
};


#endif //DXGTAPHICSCLASS_H