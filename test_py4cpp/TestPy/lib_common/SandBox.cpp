#include "stdafx.h"
#include "SandBox.h"

CSandBox::CSandBox()
{
	m_job_obj = NULL;
}

CSandBox::~CSandBox()
{
	close();
}

long CSandBox::getIocpKey()
{
	static volatile long g_job_iocp_key = 0;
	return ::InterlockedIncrement(&g_job_iocp_key);
}

bool CSandBox::init()
{
	m_job_obj = ::CreateJobObject(NULL, NULL);
	if (m_job_obj == NULL)
		return false;
	return true;
}

bool CSandBox::setLimit(int tiLimit, int memLimit)
{
	if (m_job_obj == NULL)
		return false;

	bool b_ret = true;
	b_ret = b_ret && setBaseLimit(tiLimit, memLimit);
	b_ret = b_ret && setBaseUILimit();
	b_ret = b_ret && setCallBack();
	return b_ret;
}

bool CSandBox::assignProcess(CProcBase& hProc)
{
	m_process = &hProc;
	if(::AssignProcessToJobObject(m_job_obj, m_process->getProcessHandle()))
		return true;
	return false;
}

bool CSandBox::close()
{
	::CloseHandle(m_job_obj);
	m_job_obj = NULL;
	return true;
}

bool CSandBox::setBaseLimit( int tiLimit, int memLimit )
{
	if (m_job_obj == NULL)
		return false;

	bool b_ret = true;
	
	__int64 tiLimit_100ns	= (__int64)10000 * tiLimit;
	__int64 memLimit_1b		= (__int64)1024 * memLimit;

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION	job_liex	= {0};
	JOBOBJECT_BASIC_LIMIT_INFORMATION&		job_li	= job_liex.BasicLimitInformation;
	job_li.LimitFlags = JOB_OBJECT_UILIMIT_NONE;
	job_li.LimitFlags |= JOB_OBJECT_LIMIT_PRIORITY_CLASS;
	job_li.LimitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
	job_li.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;
	job_li.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
	job_li.LimitFlags |= JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	job_li.LimitFlags |= JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;

	job_li.PriorityClass				= IDLE_PRIORITY_CLASS;	//
	job_li.ActiveProcessLimit			= 1;					//
	job_li.PerJobUserTimeLimit.QuadPart	= tiLimit_100ns;		//
	job_liex.JobMemoryLimit				= memLimit_1b;				//

	b_ret = b_ret && ::SetInformationJobObject(
		m_job_obj, JobObjectExtendedLimitInformation,
		&job_liex, sizeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION));

	// 
	JOBOBJECT_END_OF_JOB_TIME_INFORMATION job_ti = {0};
	job_ti.EndOfJobTimeAction = JOB_OBJECT_POST_AT_END_OF_JOB;

	b_ret = b_ret && ::SetInformationJobObject(
		m_job_obj, JobObjectEndOfJobTimeInformation,
		&job_ti, sizeof(JOBOBJECT_END_OF_JOB_TIME_INFORMATION));

	return b_ret;
}

bool CSandBox::setBaseUILimit()
{
	if (m_job_obj == NULL)
		return false;

	bool b_ret = true;

	JOBOBJECT_BASIC_UI_RESTRICTIONS job_uir;
	job_uir.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;
	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;			
	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_READCLIPBOARD ;	
	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_WRITECLIPBOARD ;	
	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS;	

	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DISPLAYSETTINGS;	
	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_GLOBALATOMS;		
	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DESKTOP;			
	job_uir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;		

	b_ret = b_ret && ::SetInformationJobObject(
		m_job_obj, JobObjectBasicUIRestrictions,
		&job_uir, sizeof(JOBOBJECT_BASIC_UI_RESTRICTIONS));

	return b_ret;
}

bool CSandBox::setCallBack()
{
	if (m_job_obj == NULL)
		return false;

	bool b_ret = true;

	m_job_iocp_key = getIocpKey();
	m_job_iocp = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		m_job_iocp_key,
		0);
	if (NULL == m_job_iocp)
		return false;

	JOBOBJECT_ASSOCIATE_COMPLETION_PORT job_cp = {0};
	job_cp.CompletionKey	= reinterpret_cast<void*>(m_job_iocp_key);
	job_cp.CompletionPort	= m_job_iocp;
	b_ret = b_ret && ::SetInformationJobObject(
		m_job_obj, JobObjectAssociateCompletionPortInformation,
		&job_cp, sizeof(JOBOBJECT_ASSOCIATE_COMPLETION_PORT));
	
	return b_ret;
}

int CSandBox::getStatus(int nMilliSeconds/* = INFTIME*/)
{
	bool b_ret		= true;
	int job_res		= -1;
	int proc_status	= PROC_STATUS_PROC_RUNNING;

	LPDWORD ProcessId = 0;
	b_ret = b_ret && ::GetQueuedCompletionStatus(
		m_job_iocp, (LPDWORD)&job_res, (PULONG_PTR)&m_job_iocp_key,
		(LPOVERLAPPED*)&ProcessId, nMilliSeconds);

	if (!b_ret)
	{
		if (m_process->getExitCode() != STILL_ACTIVE)
			return PROC_STATUS_ERROR_STATUS;
		return proc_status;
	}

	switch (job_res)
	{
	case JOB_OBJECT_MSG_EXIT_PROCESS:
	case JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO:
		proc_status = PROC_STATUS_PROC_EXIT;
		break;
	case JOB_OBJECT_MSG_NEW_PROCESS:
		break;
	case JOB_OBJECT_MSG_END_OF_JOB_TIME:
	case JOB_OBJECT_MSG_END_OF_PROCESS_TIME:
		proc_status = PROC_STATUS_TIME_LIMITED;
		break;
	case JOB_OBJECT_MSG_JOB_MEMORY_LIMIT:
	case JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT:
		proc_status = PROC_STATUS_MEMORY_LIMITED;
		break;
	case JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT:
	case JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS:
		proc_status = PROC_STATUS_RUNTIME_ERROR;
		break;
	default:
		proc_status = PROC_STATUS_ERROR_STATUS;
		break;
	}

	if (proc_status != PROC_STATUS_PROC_RUNNING)
	{
		m_process->terminate();
	}

	return proc_status;
}

int CSandBox::getExitCode()
{
	return m_process->getExitCode();
}
