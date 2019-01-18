#include "DxSysClass.h"


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdShow) {
	DxSysClass* sysIns;
	bool isSuccess;

	sysIns = new DxSysClass();
	if (!sysIns) {
		return 0;
	}

	isSuccess = sysIns->Init();
	if (isSuccess) {
		sysIns->Run();
	}

	sysIns->ShutDown();

	delete sysIns;
	sysIns = 0;

	return 0;
}