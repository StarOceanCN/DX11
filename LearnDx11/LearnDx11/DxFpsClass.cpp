#include "DxFpsClass.h"

DxFpsClass::DxFpsClass(){
}
DxFpsClass::DxFpsClass(const DxFpsClass& other){
}
DxFpsClass::~DxFpsClass(){
}

void DxFpsClass::Init(){
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
}

void DxFpsClass::Frame(){
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000)){
		m_fps = m_count;
		m_count = 0;
		m_startTime = timeGetTime();
	}
}
int DxFpsClass::GetFps()
{
	return m_fps;
}
