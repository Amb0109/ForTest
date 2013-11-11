#include "stdafx.h"
#include "ProcBase.h"

HANDLE CreateSandBox()
{
	HANDLE hjob = ::CreateJobObject(NULL,NULL);
	if(hjob!=NULL)
	{
		JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = {0};
		jobli.LimitFlags=JOB_OBJECT_LIMIT_PRIORITY_CLASS|JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
		jobli.PriorityClass=IDLE_PRIORITY_CLASS;	// �������ȼ�
		jobli.ActiveProcessLimit=1;					// ��������
		if(SetInformationJobObject(hjob,JobObjectBasicLimitInformation,&jobli,sizeof(jobli)))
		{
			JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir;
			jobuir.UIRestrictionsClass=JOB_OBJECT_UILIMIT_NONE;
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;			// ��ֹ����Job��ľ��
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_READCLIPBOARD ;	// ��ֹ���ʼ�����
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_WRITECLIPBOARD ;	// ��ֹ�޸ļ�����
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS;	// ��ֹ�鿴ϵͳ����

			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DISPLAYSETTINGS;	// ��ֹ�޸�ϵͳ��ʾ����
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_GLOBALATOMS;		// ��ֹ����ȫ��ԭ�ӱ�
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DESKTOP;			// ��ֹ��������
			jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;		// ��ֹ�ػ�

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

// �û�����
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

// ��������Ȩ��
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
	
	// Ĭ�������ֻ�� System �ž���SE_TCB_NAME ��Ȩ�ޣ�Admin�˺Ŷ�û�е�
	// �� Windows �������ķ�ʽ���п��Եõ� SE_TCB_NAME Ȩ��
	// �Ƿ�����Ҫͨ���޸ı��ذ�ȫ���Եõ� SE_TCB_NAME Ȩ��

	// ���� LogonUser ��Ҫ����ӵ�� SE_TCB_NAME Ȩ��
	bResult = SetPrivilege(hPowerToken, SE_TCB_NAME, TRUE, ptTemp);

	// ���� CreateProcessAsUser ��Ҫ SE_ASSIGNPRIMARYTOKEN_NAME �� SE_INCREASE_QUOTA_NAME ����Ȩ��
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

