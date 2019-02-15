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

//ģ�������ɫ����ĳ�ʼ��
bool DxGraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd) {
	//3d��س�ʼ��
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

	//��ģ�ͳ�ʼ��
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
	//��λ����س�ʼ��
	m_treePositionX = new int[m_treeNum];
	m_treePositionZ = new int[m_treeNum];
	for (int i = 0; i < m_treeNum; i++) {
		m_treePositionX[i] = rand() % 300 - 150;
		m_treePositionZ[i] = rand() % 300 - 150;
	}

	//��ģ�ͳ�ʼ��
	m_floor = new DxModelClass();
	if (!m_floor) {
		return false;
	}
	isSuccess = m_floor->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Floor.txt", L"../LearnDx11/Texture/rock.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	//���ϲ�
	m_vehicleTop = new DxModelClass();
	if (!m_vehicleTop) {
		return false;
	}
	isSuccess = m_vehicleTop->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Cube.txt", L"../LearnDx11/Texture/fir.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	//������
	m_vehicleBottom = new DxModelClass();
	if (!m_vehicleBottom) {
		return false;
	}
	isSuccess = m_vehicleBottom->Init(m_dx3dcls->GetDevice(), "../LearnDx11/Model/Cube.txt", L"../LearnDx11/Texture/sec.jpg");
	if (!isSuccess) {
		MessageBox(hwnd, L"Cant initialize the model object", L"Error", MB_OK);
		return false;
	}
	//����
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

	//���Ͻ���������
	m_text->SetMousePosition(mouseX, mouseY, m_dx3dcls->GetDeviceContext());
	m_text->SetCpu(cpuUsage, m_dx3dcls->GetDeviceContext());
	m_text->SetFps(fps, m_dx3dcls->GetDeviceContext());

	bool isSuccess = Render(movePosition, isFirst, vehicleDir);
	if (!isSuccess)
		return false;
	return true;
}

//movePosition �Ǿ�������������ؾ�λ�ã�isFIrst���Ƿ��ǵ�һ�˳ƣ�vehicleDir���ؾ����߷���1Ϊ��ǰ��-1Ϊ��0Ϊ���������ڼ�����̥��ת
bool DxGraphicsClass::Render(DxMoveClass* movePosition, bool isFirst, int vehicleDir) {

	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	D3DXMATRIX orthoMatrix;
	D3DXMATRIX transformMatrix;
	D3DXVECTOR3 cameraPosition;

	//�����ȡģ�ͺ������λ��
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	float cameraOffsetY;
	float cameraOffsetZ;

	//������һ�˳�ƫ������
	cameraOffsetY = isFirst ? 6.0f : 6.0f;
	cameraOffsetZ = isFirst ? 2.0f : -20.0f;

	movePosition->GetPosition(posX, posY, posZ);
	movePosition->GetRotation(rotX, rotY, rotZ);


	bool isSuccess;
	m_dx3dcls->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->SetPosition(posX, posY + cameraOffsetY, posZ + cameraOffsetZ);
	//m_camera->SetPosition(0.0f, 300.0f, 0.0f);
	//m_camera->SetRotation(30.0f, 0.0f, 0.0f);

	//��һ�˳�����ͷ��ת
	if (isFirst) { m_camera->SetRotation(rotX , rotY, rotZ); }
	else { m_camera->SetRotation(0.0f, 0.0f, 0.0f); }

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
	//��ȡһ��֮ǰ���뵽�ı���Ⱦ�е��������ʼview������Ҫ��д�޸Ľṹ
	D3DXMATRIX UIRenderView;
	m_text->GetBaseViewMatrix(UIRenderView);
	//UI��Ⱦ
	isSuccess = m_2dShader->Render(m_dx3dcls->GetDeviceContext(), m_bitmap->GetIndexCount(), worldMatrix, UIRenderView, orthoMatrix, m_bitmap->GetTexture(), NULL);
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
	m_vehicleTop->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTop->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicleTop->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	
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
	m_vehicleBottom->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleBottom->GetIndexCount(),
		vehicleWorldMatrix, viewMatrix, projectionMatrix, m_vehicleBottom->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	D3DXMATRIX t1, t2, t3, t4;
	vehicleWorldMatrix = worldMatrix;
	//����
	D3DXMatrixScaling(&transformMatrix, 2.0f, 1.0f, 2.0f);
	vehicleWorldMatrix *= transformMatrix;
	//��ת
	D3DXMatrixRotationZ(&transformMatrix, D3DX_PI / 2);
	vehicleWorldMatrix *= transformMatrix;
	//������ת������vehicleDir��1��-1��0��
	static int ra = 0;
	ra += (5*vehicleDir);
	ra %= 360;
	D3DXMatrixRotationX(&transformMatrix, ra * 0.0174532925f);
	vehicleWorldMatrix *= transformMatrix;
	//�ĸ�����λ��
	D3DXMatrixTranslation(&t1, 2.0f, 1.0f, 2.0f);
	D3DXMatrixTranslation(&t2, -2.0f, 1.0f, 2.0f);
	D3DXMatrixTranslation(&t3, 2.0f, 1.0f, -2.0f);
	D3DXMatrixTranslation(&t4, -2.0f, 1.0f, -2.0f);

	t1 = vehicleWorldMatrix * t1;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t1 *= transformMatrix;
	//ƽ��
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t1 *= transformMatrix;

	//��Ⱦ����1
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t1, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	t2 = vehicleWorldMatrix * t2;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t2 *= transformMatrix;
	//ƽ��
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t2 *= transformMatrix;

	//��Ⱦ����2
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t2, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	t3 = vehicleWorldMatrix * t3;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t3 *= transformMatrix;
	//ƽ��
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t3 *= transformMatrix;

	//��Ⱦ����3
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t3, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());

	t4 = vehicleWorldMatrix * t4;
	D3DXMatrixRotationY(&transformMatrix, rotY*0.0174532925f);
	t4 *= transformMatrix;
	//ƽ��
	D3DXMatrixTranslation(&transformMatrix, posX, posY, posZ);
	t4 *= transformMatrix;

	//��Ⱦ����4
	m_vehicleTire->Render(m_dx3dcls->GetDeviceContext());
	isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_vehicleTire->GetIndexCount(),
		t4, viewMatrix, projectionMatrix, m_vehicleTire->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
	
	//װ�λ���
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

	//װ���̴�
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

	//������Ⱦ����

	D3DXMATRIX treeWorldMatrix;
	//��Ⱦ��
	for (int i = 0; i < m_treeNum; i++) {
		D3DXMATRIX temp;
		//����
		treeWorldMatrix = worldMatrix;
		//����
		D3DXMatrixScaling(&temp, 2.0f, 7.0f, 2.0f);
		treeWorldMatrix *= temp;
		//��ת
		D3DXMatrixRotationZ(&temp, D3DX_PI);
		treeWorldMatrix *= temp;
		//ƽ��
		D3DXMatrixTranslation(&temp, m_treePositionX[i], 4.0f,m_treePositionZ[i]);
		treeWorldMatrix *= temp;
		m_treeBottom->Render(m_dx3dcls->GetDeviceContext());
		isSuccess = m_modelShader->Render(m_dx3dcls->GetDeviceContext(), m_treeBottom->GetIndexCount(),
			treeWorldMatrix, viewMatrix, projectionMatrix, m_treeBottom->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor());
		//����
		treeWorldMatrix = worldMatrix;
		//����
		D3DXMatrixScaling(&temp, 2.0f, 2.0f, 2.0f);
		treeWorldMatrix *= temp;
		//ƽ��
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