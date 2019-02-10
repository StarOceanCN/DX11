#include"DxLightClass.h"

DxLightClass::DxLightClass()
{
}

DxLightClass::DxLightClass(const DxLightClass& other) {

}

DxLightClass::~DxLightClass()
{
}

void DxLightClass::SetDiffuseColor(float r, float g, float b, float a) {
	m_diffuseColor = D3DXVECTOR4(r, g, b, a);
}


void DxLightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}


D3DXVECTOR4 DxLightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}


D3DXVECTOR3 DxLightClass::GetDirection()
{
	return m_direction;
}