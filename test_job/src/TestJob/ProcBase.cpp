#include "stdafx.h"
#include "ProcBase.h"

CProcBase::CProcBase()
{
	ZeroMemory( &m_start_info, sizeof(STARTUPINFO));
	m_start_info.cb			= sizeof(STARTUPINFO);
	m_start_info.hStdError	= ::GetStdHandle(STD_ERROR_HANDLE);
	m_start_info.hStdOutput	= ::GetStdHandle(STD_OUTPUT_HANDLE);
	m_start_info.hStdInput	= ::GetStdHandle(STD_INPUT_HANDLE);
	m_start_info.dwFlags	|= STARTF_USESTDHANDLES;

	m_use_input_file	= false;
	m_use_output_file	= false;
	m_use_error_file	= false;
}

CProcBase::~CProcBase()
{
	Terminate();
	SetInputFile();
	SetOutputFile();
	SetErrorFile();
	::CloseHandle(m_proc_info.hThread);
	::CloseHandle(m_proc_info.hProcess);
}

bool CProcBase::Create(const char* sCmdLine)
{
	char sDCmdLine[1024];

	strcpy(sDCmdLine, sCmdLine);

	BOOL b_ret = CreateProcess(
		NULL,
		sDCmdLine,
		NULL,
		NULL,
		TRUE,
		//CREATE_NO_WINDOW | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB ,
		CREATE_NEW_CONSOLE | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB ,
		NULL,
		NULL,
		&m_start_info,
		&m_proc_info);

	SetInputFile();
	SetOutputFile();
	SetErrorFile();

	return b_ret == TRUE;
}


bool CProcBase::CreateAsUser( HANDLE hToken, const char* sCmdLine )
{
	char sDCmdLine[1024];

	strcpy(sDCmdLine, sCmdLine);

	BOOL b_ret = CreateProcessAsUser(
		hToken,
		NULL,
		sDCmdLine,
		NULL,
		NULL,
		TRUE,
		//CREATE_NO_WINDOW | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB ,
		CREATE_NEW_CONSOLE | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB ,
		NULL,
		NULL,
		&m_start_info,
		&m_proc_info);

	SetInputFile();
	SetOutputFile();
	SetErrorFile();

	return b_ret == TRUE;
}


int CProcBase::Run(long nMilliSeconds /* = INFTIME */)
{
	if ( ::ResumeThread(m_proc_info.hThread) == -1)
		return -1;

	DWORD b_ret = ::WaitForSingleObject(m_proc_info.hProcess, nMilliSeconds);

	switch( b_ret )
	{
	case WAIT_OBJECT_0:
		return 1;
	case WAIT_TIMEOUT:
		return 0;
	case WAIT_FAILED:
		return -1;
	}
	return 0;
}

unsigned CProcBase::GetExitCode()
{
	DWORD _exit_code = 0;
	::GetExitCodeProcess(m_proc_info.hProcess, &_exit_code);
	return _exit_code;
}

bool CProcBase::Terminate()
{
	TerminateProcess(m_proc_info.hProcess, 1);
	return true;
}

//nTIME CProcBase::getTime()
//{
//	return BaseFunc::GetProcessTime(m_proc_info.hProcess);
//}
//
//nSIZE CProcBase::getMemory()
//{
//	return BaseFunc::GetProcessMemory(m_proc_info.hProcess);
//}

bool CProcBase::SetInputFile(const char* sFilePath/* = NULL*/)
{
	if (sFilePath == NULL)
	{
		if (m_use_input_file)
			::CloseHandle(m_start_info.hStdInput);

		m_use_input_file = false;
		m_start_info.hStdInput = ::GetStdHandle(STD_INPUT_HANDLE);
		return true;
	}

	SECURITY_ATTRIBUTES saAttr = {0};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	HANDLE hFile = CreateFile(
		sFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		&saAttr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	m_use_input_file = true;
	m_start_info.hStdInput = hFile;
	return true;
}

bool CProcBase::SetOutputFile(const char* sFilePath/* = NULL*/)
{
	if (sFilePath == NULL)
	{
		if (m_use_output_file)
			::CloseHandle(m_start_info.hStdOutput);

		m_use_output_file = false;
		m_start_info.hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
		return true;
	}

	SECURITY_ATTRIBUTES saAttr = {0};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	HANDLE hFile = CreateFile(
		sFilePath,
		GENERIC_WRITE,
		NULL,
		&saAttr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	m_use_output_file = true;
	m_start_info.hStdOutput = hFile;
	return true;
}

bool CProcBase::SetErrorFile(const char* sFilePath/* = NULL*/)
{
	if (sFilePath == NULL)
	{
		if (m_use_error_file)
			::CloseHandle(m_start_info.hStdError);

		m_use_error_file = false;
		m_start_info.hStdError = ::GetStdHandle(STD_ERROR_HANDLE);
		return true;
	}

	SECURITY_ATTRIBUTES saAttr = {0};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	HANDLE hFile = CreateFile(
		sFilePath,
		GENERIC_WRITE,
		NULL,
		&saAttr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	m_use_error_file = true;
	m_start_info.hStdError = hFile;
	return true;
}
