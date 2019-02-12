#ifndef DXTIMERCLASS_H
#define DXTIMERCLASS_H

#include <windows.h>
class DxTimerClass
{
public:
	DxTimerClass();
	DxTimerClass(const DxTimerClass& );
	~DxTimerClass();

	bool Init();
	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif //DXTIMERCLASS_H
