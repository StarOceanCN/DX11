#include "DxGraphicsClass.h"

DxGraphicsClass::DxGraphicsClass() {}
DxGraphicsClass::DxGraphicsClass(const DxGraphicsClass& other) {}
DxGraphicsClass::~DxGraphicsClass() {}

bool DxGraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd) {
	return true;
}

void DxGraphicsClass::ShutDown() {
}

bool DxGraphicsClass::Frame() {
	return true;
}

bool DxGraphicsClass::Render() {
	return true;
}