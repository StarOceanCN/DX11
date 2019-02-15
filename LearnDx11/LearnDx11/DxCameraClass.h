#ifndef DXCAMERACLASS_H
#define DXCAMERACLASS_H

#include<D3DX10math.h>
/*
* 摄像机类，可获取设置好的view矩阵
*/
const float TORADIANS = 0.0174532925f;

class DxCameraClass
{
public:
	DxCameraClass();
	DxCameraClass(const DxCameraClass& other);
	~DxCameraClass();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX& viewMatrix);
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};
#endif // !DXCAMERACLASS_H

