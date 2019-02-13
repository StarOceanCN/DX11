
#include"DxMoveClass.h"

DxMoveClass::DxMoveClass(){
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed = 0.0f;
	m_backwardSpeed = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
}

DxMoveClass::DxMoveClass(const DxMoveClass& other){
}

DxMoveClass::~DxMoveClass(){
}

void DxMoveClass::SetPosition(float x, float y, float z){
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void DxMoveClass::SetRotation(float x, float y, float z){
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

void DxMoveClass::GetPosition(float& x, float& y, float& z){
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
}

void DxMoveClass::GetRotation(float& x, float& y, float& z){
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
}

void DxMoveClass::SetFrameTime(float time){
	m_frameTime = time;
}


//����ǰ���ٶ� �� ����λ��
void DxMoveClass::MoveForward(bool keydown){
	float radians;


	//��ס��ǰ��ť����ǰ���ٶȣ��ɿ����ٶȽ���
	if (keydown){
		m_forwardSpeed += m_frameTime * 0.001f;

		if (m_forwardSpeed > (m_frameTime * 0.03f)){
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}
	else{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if (m_forwardSpeed < 0.0f){
			m_forwardSpeed = 0.0f;
		}
	}

	// ����ת�Ƕ�
	radians = m_rotationY * 0.0174532925f;

	// ����λ��
	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}
//����s��������ٶ� �� ����λ��
void DxMoveClass::MoveBackward(bool keydown){
	float radians;

	if (keydown){
		m_backwardSpeed += m_frameTime * 0.001f;

		if (m_backwardSpeed > (m_frameTime * 0.03f)){
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else{
		m_backwardSpeed -= m_frameTime * 0.0007f;

		if (m_backwardSpeed < 0.0f){
			m_backwardSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

}

//��ת�ٶȼ��� �� ������ת��
void DxMoveClass::TurnLeft(bool keydown){
	if (keydown){
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f)){
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else{
		m_leftTurnSpeed -= m_frameTime * 0.005f;

		if (m_leftTurnSpeed < 0.0f){
			m_leftTurnSpeed = 0.0f;
		}
	}

	//������ת��
	m_rotationY -= m_leftTurnSpeed;

	//������ת�Ƿ�Χ
	if (m_rotationY < 0.0f){
		m_rotationY += 360.0f;
	}
}

void DxMoveClass::TurnRight(bool keydown){

	if (keydown){
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f)){
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else{
		m_rightTurnSpeed -= m_frameTime * 0.005f;

		if (m_rightTurnSpeed < 0.0f){
			m_rightTurnSpeed = 0.0f;
		}
	}

	m_rotationY += m_rightTurnSpeed;

	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}
}