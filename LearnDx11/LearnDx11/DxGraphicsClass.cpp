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
	//摄像机初始化
	m_camera = new DxCameraClass();
	if (!m_camera)
		return false;
	m_camera->SetPosition(0.0f, 1.0f, -10.0f);
	m_camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_camera->Render();
	D3DXMATRIX baseViewMatrix;
	m_camera->GetViewMatrix(baseViewMatrix);


	//模型初始化
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


	//天空盒子初始化
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

	//光源初始化
	m_light = new DxLightClass();
	if (!m_light) {
		return false;
	}
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f,1.0f, 1.0f, 1.0f);
	m_light->SetDirection(-1.0f, -1.0f, 1.0f);

	//2dUI模型初始化
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

	//文本初始化
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

	//输入获取模型和摄像机位置
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

	//第一人称摄像头旋转
	if (isFirst) { m_camera->SetRotation(rotX, rotY, rotZ); }

	m_camera->Render();
	//获取四个个矩阵
	m_camera->GetViewMatrix(viewMatrix);
	m_dx3dcls->GetWorldMatrix(worldMatrix);
	m_dx3dcls->GetOrthoMatrix(orthoMatrix);
	m_dx3dcls->GetProjectionMatrix(projectionMatrix);


	//获取摄像机位置用于天空盒
	cameraPosition = m_camera->GetPosition();
	//天空盒环绕摄像机
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	//关闭z缓冲
	m_dx3dcls->ZBufferTurnOff();
	//关闭面剔除
	m_dx3dcls->CullingTurnOff();
	//天空盒渲染
	m_skybox->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_skyboxShader->Render(m_dx3dcls->GetDeviceContext(), m_skybox->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_skybox->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	if (!isSuccess)
		return false;
	//打开剔除
	m_dx3dcls->CullingTurnOn();

	//重置世界坐标位置矩阵
	m_dx3dcls->GetWorldMatrix(worldMatrix);
	//设置2dUI位置并渲染，0，0 为左上角
	isSuccess = m_bitmap->Render(m_dx3dcls->GetDeviceContext(), 0, 0);
	if (!isSuccess){
		return false;
	}
	//UI渲染
	isSuccess = m_2dShader->Render(m_dx3dcls->GetDeviceContext(), m_bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_bitmap->GetTexture(), NULL);
	if (!isSuccess){
		return false;
	}


	//开启混合用于文本渲染
	m_dx3dcls->AlphaBlendingTurnOn();
	//渲染文本
	isSuccess = m_text->Render(m_dx3dcls->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!isSuccess){
		return false;
	}
	//关闭混合
	m_dx3dcls->AlphaBlendingTurnOff();
	//开启z缓冲
	m_dx3dcls->ZBufferTurnOn();

	//地板渲染相关变换
	D3DXMATRIX floorWorldMatrix;
	floorWorldMatrix = worldMatrix;
	//缩放地板
	D3DXMatrixScaling(&transformMatrix, 150.0f, 1.0f, 150.0f);
	floorWorldMatrix *= transformMatrix;
	//平移
	//D3DXMatrixTranslation(&transformMatrix, 0.0f, 0.0f, 0.0f);
	//floorWorldMatrix *= transformMatrix;
	//渲染
	m_floor->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_floor->GetIndexCount(),
		floorWorldMatrix, viewMatrix, projectionMatrix, m_floor->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	if (!isSuccess)
		return false;

	//车子渲染相关
	D3DXMATRIX vehicleWorldMatrix;
	//初始位置被DxMoveClass决定

	vehicleWorldMatrix = worldMatrix;

	//缩放
	D3DXMatrixScaling(&transformMatrix, 2.0f, 1.0f, 4.0f);
	vehicleWorldMatrix *= transformMatrix;
	//旋转
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	vehicleWorldMatrix *= transformMatrix;
	//平移
	D3DXMatrixTranslation(&transformMatrix, posX, posY + 2.0f, posZ);
	vehicleWorldMatrix *= transformMatrix;
	//渲染
	m_vehicle_top->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicle_top->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicle_top->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	
	vehicleWorldMatrix = worldMatrix;
	//缩放
	D3DXMatrixScaling(&transformMatrix, 1.5f, 1.0f, 2.0f);
	vehicleWorldMatrix *= transformMatrix;
	//旋转
	D3DXMatrixRotationY(&transformMatrix, rotY * 0.0174532925f);
	vehicleWorldMatrix *= transformMatrix;
	//平移
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