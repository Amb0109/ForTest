#include "stdafx.h"
#include "ProcBase.h"

HANDLE CreateSandBox()
{
	HANDLE hjob = ::CreateJobObject(NULL,NULL);
	if(hjob!=NULL)
	{
		JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = {0};
		jobli.LimitFlags=JOB_OBJECT_LIMIT_PRIORITY_CLASS|JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
		jobli.PriorityClass=IDLE_PRIORITY_CLASS;	// 进程优先级
		jobli.ActiveProcessLimit=1;					// 进程数量
		if(SetInformationJobObject(hjob,JobObjectBasicLimitInformation,&jobli,sizeof(jobli)))
		{
			JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir;
			jobuir.UIRestrictionsClass=JOB_OBJECT_UILIMIT_NONE;
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;			// 禁止访问Job外的句柄
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_READCLIPBOARD ;	// 禁止访问剪贴板
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_WRITECLIPBOARD ;	// 禁止修改剪贴板
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS;	// 禁止查看系统参数

			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DISPLAYSETTINGS;	// 禁止修改系统显示设置
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_GLOBALATOMS;		// 禁止访问全局原子表
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DESKTOP;			// 禁止创建桌面
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;		// 禁止关机

			if(SetInformationJobObject(hjob,JobObjectBasicUIRestrictions,&jobuir,sizeof(jobuir)))
			{
				return hjob;
			}

		}

	}

	return NULL;
}

bool ProcessToSandbox(HANDLE job,PROCESS_INFORMATION p)
{
	if(AssignProcessToJobObject(job,p.hProcess))
		return true;
	return false;
}

#define SAFE_ACCOUNT "JUDGE"
#define SAFE_ACCOUNT_PASS "__JUDGE__"

// 用户令牌
BOOL CreateSafeToken(HANDLE &hSafeToken)
{
	if(!LogonUser(SAFE_ACCOUNT,
		NULL,
		SAFE_ACCOUNT_PASS,
		LOGON32_LOGON_INTERACTIVE,
		LOGON32_PROVIDER_DEFAULT,
		&hSafeToken)) 
		return FALSE;
	return TRUE;
}

// 调整进程权限
BOOL SetPrivilege(HANDLE hToken,
					 LPCTSTR lpPrivilegeName,
					 BOOL bEnable,
					 TOKEN_PRIVILEGES &tpOld)
{ 
	TOKEN_PRIVILEGES tp;
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

BOOL EnablePowerPrivileges(HANDLE& hPowerToken)
{
	TOKEN_PRIVILEGES ptTemp;
	BOOL bResult = TRUE;
	
	// 默认情况下只有 System 才具有SE_TCB_NAME 的权限，Admin账号都没有的
	// 以 Windows 服务程序的方式运行可以得到 SE_TCB_NAME 权限
	// 非服务需要通过修改本地安全策略得到 SE_TCB_NAME 权限

	// 调用 LogonUser 需要进程拥有 SE_TCB_NAME 权限
	bResult = SetPrivilege(hPowerToken, SE_TCB_NAME, TRUE, ptTemp);

	// 调用 CreateProcessAsUser 需要 SE_ASSIGNPRIMARYTOKEN_NAME 和 SE_INCREASE_QUOTA_NAME 两个权限
	bResult = bResult && SetPrivilege(hPowerToken, SE_INCREASE_QUOTA_NAME, TRUE, ptTemp);
	bResult = bResult && SetPrivilege(hPowerToken, SE_ASSIGNPRIMARYTOKEN_NAME, TRUE, ptTemp);

	//bResult = bResult && SetPrivilege(hPowerToken, SE_DEBUG_NAME, TRUE, ptTemp);

	return bResult;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hPowerToken	= NULL;
	HANDLE hUserToken	= NULL;
	::OpenProcessToken(::GetCurrentProcess(), TOKEN_ALL_ACCESS, &hPowerToken);
	EnablePowerPrivileges(hPowerToken);

	CreateSafeToken(hUserToken);

	CProcBase proc;
	//proc.Create("C:\\Python27\\python.exe");
	//proc.CreateAsUser(hUserToken, "C:\\Python27\\python.exe");
	proc.CreateAsUser(hUserToken, "python");

	//ThreadImpersonate(proc.getProcInfo(), SecurityImpersonation);

	HANDLE job = CreateSandBox();

	ProcessToSandbox(job, proc.getProcInfo());
	

	proc.Run(0);

	while(true);

	return 0;
}

