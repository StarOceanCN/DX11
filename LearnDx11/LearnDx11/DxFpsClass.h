#ifndef DXFPSCLASS_H
#define DXFPSCLASS_H

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class DxFpsClass
{
public:
	DxFpsClass();
	DxFpsClass(const DxFpsClass& other);
	~DxFpsClass();

	void Init();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif //DXFPSCLASS_H