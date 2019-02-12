
#ifndef DXCPUCLASS_H
#define DXCPUCLASS_H

//获取cpu信息的库
#pragma comment(lib, "pdh.lib")
#include <pdh.h>

class DxCpuClass
{
public:
	DxCpuClass();
	DxCpuClass(const DxCpuClass& other);
	~DxCpuClass();

	void Init();
	void ShutDown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};

#endif //DXCPUCLASS_H