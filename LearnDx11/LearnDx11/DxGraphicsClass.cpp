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
	m_vehicleTop = 0;
	m_vehicleBottom = 0;
	m_vehicleTire = 0;
	m_treeBottom = 0;
	m_treeTop = 0;
	m_treePositionX = 0;
	m_treePositionZ = 0;
	m_treeNum = 30;
}
DxGraphicsClass::DxGraphicsClass(const DxGraphicsClass& other) {}
DxGraphicsClass::~DxGraphicsClass() {}

//模型类和着色器类的初始化
bool DxGraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd) {
	//3d相关初始化
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

	//树模型初始化
	m_treeBottom = new DxModelClass();
	if (!m_treeBottom) {
		return false;
	}
	isSuccess = m_treeBottom->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Tire.txt", L"../LearnDx11/Texture/rock.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	m_treeTop = new DxModelClass();
	if (!m_treeTop) {
		return false;
	}
	isSuccess = m_treeTop->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Skybox.txt", L"../LearnDx11/Texture/TreeTop.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	//树位置相关初始化
	m_treePositionX = new int[m_treeNum];
	m_treePositionZ = new int[m_treeNum];
	for (int i = 0; i < m_treeNum; i++) {
		m_treePositionX[i] = rand() % 300 - 150;
		m_treePositionZ[i] = rand() % 300 - 150;
	}

	//车模型初始化
	m_floor = new DxModelClass();
	if (!m_floor) {
		return false;
	}
	isSuccess = m_floor->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Floor.txt", L"../LearnDx11/Texture/rock.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	//车上部
	m_vehicleTop = new DxModelClass();
	if (!m_vehicleTop) {
		return false;
	}
	isSuccess = m_vehicleTop->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Cube.txt", L"../LearnDx11/Texture/fir.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	//车底盘
	m_vehicleBottom = new DxModelClass();
	if (!m_vehicleBottom) {
		return false;
	}
	isSuccess = m_vehicleBottom->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Cube.txt", L"../LearnDx11/Texture/sec.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	//轮子
	m_vehicleTire = new DxModelClass();
	if (!m_vehicleTire) {
		return false;
	}
	isSuccess = m_vehicleTire->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Tire.txt", L"../LearnDx11/Texture/Tire.png");
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
	if (m_vehicleBottom) {
		m_vehicleBottom->ShutDown();
		delete m_vehicleBottom;
		m_vehicleBottom = 0;
	}
	if (m_vehicleTop) {
		m_vehicleTop->ShutDown();
		delete m_vehicleTop;
		m_vehicleTop = 0;
	}
	if (m_vehicleTire) {
		m_vehicleTire->ShutDown();
		delete m_vehicleTire;
		m_vehicleTire = 0;
	}

	if (m_text) {
		m_text->ShutDown();
		delete m_text;
		m_text = 0;
	}
	if (m_treeBottom) {
		m_treeBottom->ShutDown();
		delete m_treeBottom;
		m_treeBottom = 0;
	}
	if (m_treeTop) {
		m_treeTop->ShutDown();
		delete m_treeTop;
		m_treeTop = 0;
	}


}

bool DxGraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpuUsage, DxMoveClass* movePosition, bool isFirst, int vehicleDir) {

	//左上角文字设置
	m_text->SetMousePosition(mouseX, mouseY, m_dx3dcls->GetDeviceContext());
	m_text->SetCpu(cpuUsage, m_dx3dcls->GetDeviceContext());
	m_text->SetFps(fps, m_dx3dcls->GetDeviceContext());

	bool isSuccess = Render(movePosition, isFirst, vehicleDir);
	if (!isSuccess)
		return false;
	return true;
}

//movePosition 是经过输入后计算的载具位置，isFIrst是是否是第一人称，vehicleDir是载具行走方向，1为向前，-1为后，0为不动，用于计算轮胎旋转
bool DxGraphicsClass::Render(DxMoveClass* movePosition, bool isFirst, int vehicleDir) {

	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	D3DXMATRIX orthoMatrix;
	D3DXMATRIX transformMatrix;
	D3DXVECTOR3 cameraPosition;

	//输入获取模型和摄像机位置
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	float cameraOffsetY;
	float cameraOffsetZ;

	//调整第一人称偏移坐标
	cameraOffsetY = isFirst ? 6.0f : 6.0f;
	cameraOffsetZ = isFirst ? 2.0f : -20.0f;

	movePosition->GetPosition(posX, posY, posZ);
	movePosition->GetRotation(rotX, rotY, rotZ);


	bool isSuccess;
	m_dx3dcls->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->SetPosition(posX, posY + cameraOffsetY, posZ + cameraOffsetZ);
	//m_camera->SetPosition(0.0f, 300.0f, 0.0f);
	//m_camera->SetRotation(30.0f, 0.0f, 0.0f);

	//第一人称摄像头旋转
	if (isFirst) { m_camera->SetRotation(rotX , rotY, rotZ); }
	else { m_camera->SetRotation(0.0f, 0.0f, 0.0f); }

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
	//获取一下之前存入到文本渲染中的摄像机初始view矩阵，需要重写修改结构
	D3DXMATRIX UIRenderView;
	m_text->GetBaseViewMatrix(UIRenderView);
	//UI渲染
	isSuccess = m_2dShader->Render(m_dx3dcls->GetDeviceContext(), m_bitmap->GetIndexCount(), worldMatrix, UIRenderView, orthoMatrix, m_bitmap->GetTexture(), NULL);
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
	m_vehicleTop->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTop->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicleTop->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	
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
	m_vehicleBottom->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleBottom->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicleBottom->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	D3DXMATRIX t1, t2, t3, t4;
	vehicleWorldMatrix = worldMatrix;
	//缩放
	D3DXMatrixScaling(&transformMatrix, 2.0f, 1.0f, 2.0f);
	vehicleWorldMatrix *= transformMatrix;
	//旋转
	D3DXMatrixRotationZ(&transformMatrix, D3DX_PI / 2);
	vehicleWorldMatrix *= transformMatrix;
	//车轮旋转变量，vehicleDir是1、-1、0；
	static int ra = 0;
	ra += (5*vehicleDir);
	ra %= 360;
	D3DXMatrixRotationX(&transformMatrix, ra * 0.0174532925f);
	vehicleWorldMatrix *= transformMatrix;
	//四个轮子位置
	D3DXMatrixTranslation(&t1, 2.0f, 1.0f, 2.0f);
	D3DXMatrixTranslation(&t2, -2.0f, 1.0f, 2.0f);
	D3DXMatrixTranslation(&t3, 2.0f, 1.0f, -2.0f);
	D3DXMatrixTranslation(&t4, -2.0f, 1.0f, -2.0f);

	t1 = vehicleWorldMatrix * t1;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t1 *= transformMatrix;
	//平移
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t1 *= transformMatrix;

	//渲染轮子1
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t1, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	t2 = vehicleWorldMatrix * t2;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t2 *= transformMatrix;
	//平移
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t2 *= transformMatrix;

	//渲染轮子2
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t2, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	t3 = vehicleWorldMatrix * t3;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t3 *= transformMatrix;
	//平移
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t3 *= transformMatrix;

	//渲染轮子3
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t3, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	t4 = vehicleWorldMatrix * t4;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t4 *= transformMatrix;
	//平移
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t4 *= transformMatrix;

	//渲染轮子4
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t4, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	
	//装饰火炮
	D3DXMATRIX t5;
	vehicleWorldMatrix = worldMatrix;
	D3DXMatrixScaling(&t5, 1.0f, 5.0f, 1.0f);
	vehicleWorldMatrix *= t5;
	D3DXMatrixRotationX(&t5, -D3DX_PI / 2);
	vehicleWorldMatrix *= t5;
	D3DXMatrixTranslation(&t5, 0.0f, 4.0f, 4.0f);
	vehicleWorldMatrix *= t5;
	D3DXMatrixRotationY(&t5, rotY*0.0174532925f);
	vehicleWorldMatrix *= t5;
	D3DXMatrixTranslation(&t5, posX, posY, posZ);
	vehicleWorldMatrix *= t5;
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	//装饰烟囱
	vehicleWorldMatrix = worldMatrix;
	D3DXMatrixScaling(&t5, 0.5f, 3.0f, 0.5f);
	vehicleWorldMatrix *= t5;
	D3DXMatrixRotationX(&t5, D3DX_PI);
	vehicleWorldMatrix *= t5;
	D3DXMatrixTranslation(&t5, 1.0f, 4.0f, -1.0f);
	vehicleWorldMatrix *= t5;
	D3DXMatrixRotationY(&t5, rotY*0.0174532925f);
	vehicleWorldMatrix *= t5;
	D3DXMatrixTranslation(&t5, posX, posY, posZ);
	vehicleWorldMatrix *= t5;
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	//车子渲染结束

	D3DXMATRIX treeWorldMatrix;
	//渲染树
	for (int i = 0; i < m_treeNum; i++) {
		D3DXMATRIX temp;
		//树干
		treeWorldMatrix = worldMatrix;
		//缩放
		D3DXMatrixScaling(&temp, 2.0f, 7.0f, 2.0f);
		treeWorldMatrix *= temp;
		//旋转
		D3DXMatrixRotationZ(&temp, D3DX_PI);
		treeWorldMatrix *= temp;
		//平移
		D3DXMatrixTranslation(&temp, m_treePositionX[i], 4.0f,m_treePositionZ[i]);
		treeWorldMatrix *= temp;
		m_treeBottom->Render(m_dx3dcls->GetDeviceContext());
		isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_treeBottom->GetIndexCount(),
			treeWorldMatrix, viewMatrix, projectionMatrix, m_treeBottom->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
		//树顶
		treeWorldMatrix = worldMatrix;
		//缩放
		D3DXMatrixScaling(&temp, 2.0f, 2.0f, 2.0f);
		treeWorldMatrix *= temp;
		//平移
		D3DXMatrixTranslation(&temp, m_treePositionX[i], 10.0f, m_treePositionZ[i]);
		treeWorldMatrix *= temp;
		m_treeTop->Render(m_dx3dcls->GetDeviceContext());
		isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_treeTop->GetIndexCount(),
			treeWorldMatrix, viewMatrix, projectionMatrix, m_treeTop->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
		if (!isSuccess)
			return false;
	}
	m_dx3dcls->EndScene();

	return true;
}