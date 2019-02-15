#ifndef DXMOVECLASS_H
#define DXMOVECLASS_H
#include <math.h>
//根据按键计算position和模型的rotation
class DxMoveClass
{
public:
	DxMoveClass();
	DxMoveClass(const DxMoveClass& other);
	~DxMoveClass();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void GetPosition(float& x, float& y, float& z);
	void GetRotation(float& x, float& y, float& z);

	void SetFrameTime(float time);

	void MoveForward(bool W);
	void MoveBackward(bool S);
	void TurnLeft(bool A);
	void TurnRight(bool D);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
};

#endif //DXMOVECLASS_H
