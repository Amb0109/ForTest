// TestWinAPI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BaseFunc.h"

// 调整进程权限
BOOL SetPrivilege(HANDLE hToken,
				  LPCTSTR lpPrivilegeName,
				  BOOL bEnable)
{ 
	TOKEN_PRIVILEGES tp;
	TOKEN_PRIVILEGES tpOld;
	DWORD cb = sizeof(tpOld);
	//look up value of the privilegename on the local system and get the luid 
	if(::LookupPrivilegeValue(NULL, lpPrivilegeName, &tp.Privileges[0].Luid)) 
	{
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		if(::AdjustTokenPrivileges(hToken, FALSE, &tp, cb, &tpOld, &cb)) 
		{
			return (ERROR_NOT_ALL_ASSIGNED != GetLastError());     
		}
	}
	return FALSE;
}

int GetProcessTime(const HANDLE hProcess)
{
	unsigned long dwTimeUsage = 0;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;

	if(GetProcessTimes(hProcess, &CreationTime, &ExitTime, &KernelTime, &UserTime))
	{
		dwTimeUsage = (KernelTime.dwHighDateTime + UserTime.dwHighDateTime) * 1000 + 
			(KernelTime.dwLowDateTime + UserTime.dwLowDateTime) / 10000;
	}
	return dwTimeUsage;
}


DWORD GetProcessMemory(const HANDLE hProcess)
{
	DWORD dwMemory = 0;
	PROCESS_MEMORY_COUNTERS pmc;

	::ZeroMemory(&pmc, sizeof(pmc));
	if(::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		dwMemory = (DWORD)pmc.PeakWorkingSetSize / 1024;
	return dwMemory;
}


std::string DosDevicePath2LogicalPath(LPCTSTR lpszDosPath)
{
	std::string strResult;

	// Translate path with device name to drive letters.
	TCHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';

	if ( lpszDosPath==NULL || !GetLogicalDriveStrings(_countof(szTemp)-1, szTemp) ){
		return strResult;
	}


	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;

	do{
		// Copy the drive letter to the template string
		*szDrive = *p;

		// Look up each device name
		if ( QueryDosDevice(szDrive, szName, _countof(szName)) ){
			UINT uNameLen = (UINT)_tcslen(szName);

			if (uNameLen < MAX_PATH) 
			{
				bFound = _tcsnicmp(lpszDosPath, szName, uNameLen) == 0;

				if ( bFound ){
					// Reconstruct pszFilename using szTemp
					// Replace device path with DOS path
					TCHAR szTempFile[MAX_PATH];
					_stprintf_s(szTempFile, TEXT("%s%s"), szDrive, lpszDosPath+uNameLen);
					strResult = szTempFile;
				}
			}
		}

		// Go to the next NULL character.
		while (*p++);
	} while (!bFound && *p); // end of string

	return strResult;
}

void GetCurrentProcessList()
{
	HANDLE hPowerToken	= NULL;

	::OpenProcessToken(::GetCurrentProcess(), TOKEN_ALL_ACCESS, &hPowerToken);
	::SetPrivilege(hPowerToken, SE_DEBUG_NAME, TRUE);

	DWORD proc_lst_size = 1024;
	DWORD proc_id_lst[1024];
	DWORD proc_id_cnt;

	::ZeroMemory(proc_id_lst, 1024);
	if( !::EnumProcesses(proc_id_lst, proc_lst_size, &proc_id_cnt) )
		return;
	proc_id_cnt /= sizeof(DWORD);

	for (int i=0; i<proc_id_cnt; ++i)
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc_id_lst[i]);
		if (hProcess)
		{
			char FileName[MAX_PATH];
			char Md5Digest[256];
			::ZeroMemory(FileName, MAX_PATH);
			::ZeroMemory(Md5Digest, 256);
			DWORD ret = ::GetProcessImageFileName(hProcess, (LPSTR)FileName, MAX_PATH);
			std::string FilePath = ::DosDevicePath2LogicalPath(FileName);
			ret = BaseFunc::MD5File(FilePath.c_str(), Md5Digest);
			printf("%d %s %s\n", proc_id_lst[i], FilePath.c_str(), Md5Digest);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	GetCurrentProcessList();
	system("pause");
	return 0;
}

