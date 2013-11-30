#pragma comment(lib,"Psapi.lib")

#include <Windows.h>
#include <psapi.h>
#include "Common.h"


// �������ʹ�õ�CPUʱ�䣨�������Եľ��亯������Ҫ�޸����е���䣩
DWORD GetProcessTime(const HANDLE& hProcess)
{
	DWORD dwTimeUsage = 0;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;

	if(GetProcessTimes(hProcess, &CreationTime, &ExitTime, &KernelTime, &UserTime))
	{
		dwTimeUsage = (KernelTime.dwHighDateTime + UserTime.dwHighDateTime) * 1000 + 
			(KernelTime.dwLowDateTime + UserTime.dwLowDateTime) / 10000;//why such calc
	}
	return dwTimeUsage;
}

// �������Եľ��亯������Ҫ�޸����е����
DWORD GetProcessMemory(const HANDLE& hProcess)
{
	DWORD dwMemory = 0;
	PROCESS_MEMORY_COUNTERS pmc;

	ZeroMemory(&pmc, sizeof(pmc));
	if(GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		dwMemory = (DWORD)pmc.PeakWorkingSetSize / 1024;
	return dwMemory;
}