#include "DxCameraClass.h"

DxCameraClass::DxCameraClass() {
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

DxCameraClass::DxCameraClass(const DxCameraClass& other) {}
DxCameraClass::~DxCameraClass() {}

void DxCameraClass::SetPosition(float x, float y, float z) {
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void DxCameraClass::SetRotation(float x, float y, float z) {
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

D3DXVECTOR3 DxCameraClass::GetPosition() {
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 DxCameraClass::GetRotation() {
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void DxCameraClass::Render() {
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	//setup the vector that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = m_rotationX * TORADIANS;
	yaw = m_rotationY * TORADIANS;
	roll = m_rotationZ * TORADIANS;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
}

void DxCameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix) {
	viewMatrix = m_viewMatrix;
}
