#include "DxGraphicsClass.h"

DxGraphicsClass::DxGraphicsClass() {
	m_dx3dcls = 0;
	m_camera = 0;
	m_skybox = 0;
	m_modelShader = 0;
	m_skyboxShader = 0;
	m_2dShader = 0;
	m_light = 0;
	m_bitmap = 0;
	m_floor = 0;
	m_text = 0;
	m_vehicle_top = 0;
	m_vehicle_bottom = 0;
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
	//�������ʼ��
	m_camera = new DxCameraClass();
	if (!m_camera)
		return false;
	m_camera->SetPosition(0.0f, 1.0f, -10.0f);
	m_camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_camera->Render();
	D3DXMATRIX baseViewMatrix;
	m_camera->GetViewMatrix(baseViewMatrix);


	//ģ�ͳ�ʼ��
	m_floor = new DxModelClass();
	if (!m_floor) {
		return false;
	}
	isSuccess = m_floor->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Floor.txt", L"../LearnDx11/Texture/rock.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}

	m_vehicle_top = new DxModelClass();
	if (!m_vehicle_top) {
		return false;
	}
	isSuccess = m_vehicle_top->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Cube.txt", L"../LearnDx11/Texture/fir.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	m_vehicle_bottom = new DxModelClass();
	if (!m_vehicle_bottom) {
		return false;
	}
	isSuccess = m_vehicle_bottom->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Cube.txt", L"../LearnDx11/Texture/sec.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	m_modelShader = new DxShaderClass();
	isSuccess = m_modelShader->Init(m_dx3dcls->GetDevice(), hwnd, L"../LearnDx11/Light.vs", L"../LearnDx11/Light.ps", "LightVertexShader", "LightPixelShader");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model shader object", L"Error", MB_OK);
		return false;
	}


	//��պ��ӳ�ʼ��
	m_skybox = new DxModelClass();
	if (!m_skybox)
		return false;

	isSuccess = m_skybox->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/SkyBox.txt",L"../LearnDx11/Texture/SkyboxTexture.dds");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the skybox object", L"Error", MB_OK);
		return false;
	}
	m_skyboxShader = new DxShaderClass();
	if (!m_skyboxShader) {
		return false;
	}
	isSuccess = m_skyboxShader->Init(m_dx3dcls->GetDevice(), hwnd, L"../LearnDx11/Skybox.vs", L"../LearnDx11/Skybox.ps", "SkyboxVertexShader", "SkyboxPixelShader");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the skybox shader object", L"Error", MB_OK);
		return false;
	}

	//��Դ��ʼ��
	m_light = new DxLightClass();
	if (!m_light) {
		return false;
	}
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f,1.0f, 1.0f, 1.0f);
	m_light->SetDirection(-1.0f, -1.0f, 1.0f);

	//2dUIģ�ͳ�ʼ��
	m_2dShader = new DxTextureShaderClass();
	if (!m_2dShader) {
		return false;
	}
	isSuccess = m_2dShader->Init(m_dx3dcls->GetDevice(), hwnd, L"../LearnDx11/Texture.vs", L"../LearnDx11/Texture.ps", "TextureVertexShader", "TexturePixelShader");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the 2dmodel shader object", L"Error", MB_OK);
		return false;
	}
	m_bitmap = new Dx2DRenderClass();
	if (!m_bitmap)
	{
		return false;
	}

	isSuccess = m_bitmap->Init(m_dx3dcls->GetDevice(), screenWidth, screenHeight, L"../LearnDx11/Texture/seafloor.gif", 100, 100);
	if (!isSuccess)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	//�ı���ʼ��
	m_text = new DxTextClass();
	if (!m_text)
	{
		return false;
	}

	// Initialize the text object.
	isSuccess = m_text->Init(m_dx3dcls->GetDevice(), m_dx3dcls->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!isSuccess)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void DxGraphicsClass::ShutDown() {
	if (m_skyboxShader) {
		m_skyboxShader->ShutDown();
		delete m_skyboxShader;
		m_skyboxShader = 0;
	}
	if (m_2dShader) {
		m_2dShader->ShutDown();
		delete m_2dShader;
		m_2dShader = 0;
	}

	if (m_bitmap){
		m_bitmap->Shutdown();
		delete m_bitmap;
		m_bitmap = 0;
	}

	if (m_light) {
		delete m_light;
		m_light = 0;
	}

	if (m_skybox) {
		m_skybox->ShutDown();
		delete m_skybox;
		m_skybox = 0;
	}
	if (m_floor) {
		m_floor->ShutDown();
		delete m_floor;
		m_floor = 0;
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
	if (m_vehicle_bottom) {
		m_vehicle_bottom->ShutDown();
		delete m_vehicle_bottom;
		m_vehicle_bottom = 0;
	}
	if (m_vehicle_top) {
		m_vehicle_top->ShutDown();
		delete m_vehicle_top;
		m_vehicle_top = 0;
	}

	if (m_text) {
		m_text->ShutDown();
		delete m_text;
		m_text = 0;
	}

}

bool DxGraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpuUsage, DxMoveClass* movePosition, bool isFirst) {

	m_text->SetMousePosition(mouseX, mouseY, m_dx3dcls->GetDeviceContext());
	m_text->SetCpu(cpuUsage, m_dx3dcls->GetDeviceContext());
	m_text->SetFps(fps, m_dx3dcls->GetDeviceContext());



	bool isSuccess = Render(movePosition, isFirst);
	if (!isSuccess)
		return false;
	return true;
}

bool DxGraphicsClass::Render(DxMoveClass* movePosition, bool isFirst) {

	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	D3DXMATRIX orthoMatrix;
	D3DXMATRIX transformMatrix;
	D3DXVECTOR3 cameraPosition;

	//�����ȡģ�ͺ������λ��
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	float cameraOffsetY;
	float cameraOffsetZ;

	cameraOffsetY = isFirst ? 1.0f : 0.1f;
	cameraOffsetZ = isFirst ? 1.0f : -20.0f;

	movePosition->GetPosition(posX, posY, posZ);
	movePosition->GetRotation(rotX, rotY, rotZ);


	bool isSuccess;
	m_dx3dcls->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->SetPosition(posX, posY + cameraOffsetY, posZ + cameraOffsetZ);

	//��һ�˳�����ͷ��ת
	if (isFirst) { m_camera->SetRotation(rotX, rotY, rotZ); }

	m_camera->Render();
	//��ȡ�ĸ�������
	m_camera->GetViewMatrix(viewMatrix);
	m_dx3dcls->GetWorldMatrix(worldMatrix);
	m_dx3dcls->GetOrthoMatrix(orthoMatrix);
	m_dx3dcls->GetProjectionMatrix(projectionMatrix);


	//��ȡ�����λ��������պ�
	cameraPosition = m_camera->GetPosition();
	//��պл��������
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	//�ر�z����
	m_dx3dcls->ZBufferTurnOff();
	//�ر����޳�
	m_dx3dcls->CullingTurnOff();
	//��պ���Ⱦ
	m_skybox->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_skyboxShader->Render(m_dx3dcls->GetDeviceContext(), m_skybox->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_skybox->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	if (!isSuccess)
		return false;
	//���޳�
	m_dx3dcls->CullingTurnOn();

	//������������λ�þ���
	m_dx3dcls->GetWorldMatrix(worldMatrix);
	//����2dUIλ�ò���Ⱦ��0��0 Ϊ���Ͻ�
	isSuccess = m_bitmap->Render(m_dx3dcls->GetDeviceContext(), 0, 0);
	if (!isSuccess){
		return false;
	}
	//UI��Ⱦ
	isSuccess = m_2dShader->Render(m_dx3dcls->GetDeviceContext(), m_bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_bitmap->GetTexture(), NULL);
	if (!isSuccess){
		return false;
	}


	//������������ı���Ⱦ
	m_dx3dcls->AlphaBlendingTurnOn();
	//��Ⱦ�ı�
	isSuccess = m_text->Render(m_dx3dcls->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!isSuccess){
		return false;
	}
	//�رջ��
	m_dx3dcls->AlphaBlendingTurnOff();
	//����z����
	m_dx3dcls->ZBufferTurnOn();

	//�ذ���Ⱦ��ر任
	D3DXMATRIX floorWorldMatrix;
	floorWorldMatrix = worldMatrix;
	//���ŵذ�
	D3DXMatrixScaling(&transformMatrix, 150.0f, 1.0f, 150.0f);
	floorWorldMatrix *= transformMatrix;
	//ƽ��
	//D3DXMatrixTranslation(&transformMatrix, 0.0f, 0.0f, 0.0f);
	//floorWorldMatrix *= transformMatrix;
	//��Ⱦ
	m_floor->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_floor->GetIndexCount(),
		floorWorldMatrix, viewMatrix, projectionMatrix, m_floor->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	if (!isSuccess)
		return false;

	//������Ⱦ���
	D3DXMATRIX vehicleWorldMatrix;
	//��ʼλ�ñ�DxMoveClass����

	vehicleWorldMatrix = worldMatrix;

	//����
	D3DXMatrixScaling(&transformMatrix, 2.0f, 1.0f, 4.0f);
	vehicleWorldMatrix *= transformMatrix;
	//��ת
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	vehicleWorldMatrix *= transformMatrix;
	//ƽ��
	D3DXMatrixTranslation(&transformMatrix, posX, posY + 2.0f, posZ);
	vehicleWorldMatrix *= transformMatrix;
	//��Ⱦ
	m_vehicle_top->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicle_top->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicle_top->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	
	vehicleWorldMatrix = worldMatrix;
	//����
	D3DXMatrixScaling(&transformMatrix, 1.5f, 1.0f, 2.0f);
	vehicleWorldMatrix *= transformMatrix;
	//��ת
	D3DXMatrixRotationY(&transformMatrix, rotY * 0.0174532925f);
	vehicleWorldMatrix *= transformMatrix;
	//ƽ��
	D3DXMatrixTranslation(&transformMatrix, posX, posY + 3.5f, posZ);
	vehicleWorldMatrix *= transformMatrix;
	m_vehicle_bottom->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicle_bottom->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicle_bottom->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	if (!isSuccess)
		return false;

	m_dx3dcls->EndScene();

	return true;
}