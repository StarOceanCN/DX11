#include "DxGraphicsClass.h"

DxGraphicsClass::DxGraphicsClass() {
	m_dx3dcls = 0;
	m_camera = 0;
	m_model = 0;
	m_ModelShader = 0;
	m_2dShader = 0;
	m_light = 0;
	m_bitmap = 0;
}
DxGraphicsClass::DxGraphicsClass(const DxGraphicsClass& other) {}
DxGraphicsClass::~DxGraphicsClass() {}

bool DxGraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd) {

	m_dx3dcls = new Dx3dClass();
	if (!m_dx3dcls) {
		return false;
	}

	bool isSuccess = m_dx3dcls->Init(screenWidth,screenHeight,true,hwnd,FULL_SCREEN, SCREEN_DEPTH,SCREEN_NEAR);
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant init Direct3d", L"Error", MB_OK);
		return false;
	}

	m_camera = new DxCameraClass();
	if (!m_camera)
		return false;
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_model = new DxModelClass();
	if (!m_model)
		return false;

	isSuccess = m_model->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Cube.txt",L"../LearnDx11/Texture/seafloor.gif");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	m_ModelShader = new DxShaderClass();
	if (!m_ModelShader) {
		return false;
	}
	isSuccess = m_ModelShader->Init(m_dx3dcls->GetDevice(), hwnd, L"../LearnDx11/Light.vs", L"../LearnDx11/Light.ps");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the shader object", L"Error", MB_OK);
		return false;
	}

	m_2dShader = new DxTextureShaderClass();
	if (!m_2dShader) {
		return false;
	}
	isSuccess = m_2dShader->Init(m_dx3dcls->GetDevice(), hwnd, L"../LearnDx11/Texture.vs", L"../LearnDx11/Texture.ps");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the shader object", L"Error", MB_OK);
		return false;
	}

	m_light = new DxLightClass();
	if (!m_light) {
		return false;
	}
	m_light->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);

	m_bitmap = new Dx2DRenderClass;
	if (!m_bitmap)
	{
		return false;
	}

	isSuccess = m_bitmap->Init(m_dx3dcls->GetDevice(), screenWidth, screenHeight, L"../LearnDx11/Texture/seafloor.gif", 256, 256);
	if (!isSuccess)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	return true;
}

void DxGraphicsClass::ShutDown() {
	if (m_ModelShader) {
		m_ModelShader->ShutDown();
		delete m_ModelShader;
		m_ModelShader = 0;
	}

	if (m_2dShader) {
		m_2dShader->ShutDown();
		delete m_2dShader;
		m_2dShader = 0;
	}

	if (m_bitmap)
	{
		m_bitmap->Shutdown();
		delete m_bitmap;
		m_bitmap = 0;
	}

	if (m_light) {
		delete m_light;
		m_light = 0;
	}

	if (m_model) {
		m_model->ShutDown();
		delete m_model;
		m_model = 0;
	}

	if (m_camera) {
		delete m_camera;
		m_camera = 0;
	}

	if (m_dx3dcls) {
		m_dx3dcls->ShutDown();
		delete m_dx3dcls;
		m_dx3dcls = 0;
	}

}

bool DxGraphicsClass::Frame() {
	static float rotation = 0.0f;
	static float move = 0.0f;

	rotation += (float)D3DX_PI*0.01f;
	move += 0.1f;

	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	bool isSuccess = Render(rotation, move);
	if (!isSuccess)
		return false;
	return true;
}

bool DxGraphicsClass::Render(float rotation, float move) {

	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	D3DXMATRIX orthoMatrix;

	bool isSuccess;

	m_dx3dcls->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_camera->GetViewMatrix(viewMatrix);
	m_dx3dcls->GetWorldMatrix(worldMatrix);
	m_dx3dcls->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_dx3dcls->ZBufferTurnOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	isSuccess = m_bitmap->Render(m_dx3dcls->GetDeviceContext(), 0, 0);
	if (!isSuccess)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	isSuccess = m_2dShader->Render(m_dx3dcls->GetDeviceContext(), m_bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_bitmap->GetTexture());
	if (!isSuccess)
	{
		return false;
	}

	m_dx3dcls->ZBufferTurnOn();


	m_dx3dcls->GetProjectionMatrix(projectionMatrix);

	D3DXMATRIX transformMatrix;
	D3DXMatrixRotationY(&transformMatrix, rotation);
	worldMatrix *= transformMatrix;
	D3DXMatrixTranslation(&transformMatrix, 0.0f, 0.0f, move);
	worldMatrix *= transformMatrix;
	

	m_model->Render(m_dx3dcls->GetDeviceContext());

	isSuccess = m_ModelShader->Render(m_dx3dcls->GetDeviceContext(), m_model->GetIndexCount(), 
		worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor());

	if (!isSuccess)
		return false;

	m_dx3dcls->EndScene();

	return true;
}