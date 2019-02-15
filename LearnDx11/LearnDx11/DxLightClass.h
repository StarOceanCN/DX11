#ifndef DXLIGHTCLASS_H
#define DXLIGHTCLASS_H

#include<D3DX10math.h>
//单独的光的类，主要是描述一个光的相关属性
class DxLightClass
{
public:
	DxLightClass();
	DxLightClass(const DxLightClass& other);
	~DxLightClass();

	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetAmbientColor();
private:
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};

#endif // !DXLIGHTCLASS_H

