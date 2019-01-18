#include"DxInputClass.h"

DxInputClass::DxInputClass() {}
DxInputClass::~DxInputClass() {}
DxInputClass::DxInputClass(const DxInputClass& other) {}
void DxInputClass::Init() {
	for (int i = 0; i < 256; i++) {
		m_keys[i] = false;
	}
}

void DxInputClass::KeyDown(unsigned int key) {
	m_keys[key] = true;
}

bool DxInputClass::IsKeyDown(unsigned int key) {
	return m_keys[key];
}

void DxInputClass::KeyUp(unsigned int key) {
	m_keys[key] = true;
}