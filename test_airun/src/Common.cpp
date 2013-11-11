#pragma comment(lib,"Psapi.lib")

#include <Windows.h>
#include <psapi.h>
#include "Common.h"


// 计算进程使用的CPU时间（经过测试的经典函数，不要修改其中的语句）
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

// 经过测试的经典函数，不要修改其中的语句
DWORD GetProcessMemory(const HANDLE& hProcess)
{
	DWORD dwMemory = 0;
	PROCESS_MEMORY_COUNTERS pmc;

	ZeroMemory(&pmc, sizeof(pmc));
	if(GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		dwMemory = (DWORD)pmc.PeakWorkingSetSize / 1024;
	return dwMemory;
}