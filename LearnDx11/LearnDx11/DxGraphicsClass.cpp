#include "DxGraphicsClass.h"

DxGraphicsClass::DxGraphicsClass() {
	m_dx3dcls = 0;
	m_camera = 0;
	m_model = 0;
	m_shader = 0;
	m_light = 0;
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

	isSuccess = m_model->Init(m_dx3dcls->GetDevice(), L"../LearnDx11/Texture/seafloor.gif");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	m_shader = new DxShaderClass();
	if (!m_shader) {
		return false;
	}

	isSuccess = m_shader->Init(m_dx3dcls->GetDevice(), hwnd, L"../LearnDx11/Light.vs", L"../LearnDx11/Light.ps");
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

	return true;
}

void DxGraphicsClass::ShutDown() {
	if (m_shader) {
		m_shader->ShutDown();
		delete m_shader;
		m_shader = 0;
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

	rotation += (float)D3DX_PI*0.01f;
	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	bool isSuccess = Render(rotation);
	if (!isSuccess)
		return false;
	return true;
}

bool DxGraphicsClass::Render(float rotation) {

	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	bool isSuccess;

	m_dx3dcls->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_camera->GetViewMatrix(viewMatrix);
	m_dx3dcls->GetWorldMatrix(worldMatrix);
	m_dx3dcls->GetProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);

	m_model->Render(m_dx3dcls->GetDeviceContext());

	isSuccess = m_shader->Render(m_dx3dcls->GetDeviceContext(), m_model->GetIndexCount(), 
		worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor());

	if (!isSuccess)
		return false;

	m_dx3dcls->EndScene();

	return true;
}