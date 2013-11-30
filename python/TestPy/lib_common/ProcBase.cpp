#include "stdafx.h"
#include "BaseFunc.h"
#include "PipeBase.h"
#include "ProcBase.h"

CProcBase::CProcBase()
{
	m_org_std_input		= ::GetStdHandle(STD_INPUT_HANDLE);
	m_org_std_output	= ::GetStdHandle(STD_OUTPUT_HANDLE);
	m_org_std_error		= ::GetStdHandle(STD_ERROR_HANDLE);

	memset(&m_start_info, 0, sizeof(STARTUPINFO));
	m_start_info.cb			= sizeof(STARTUPINFO);
	m_start_info.hStdInput	= m_org_std_input;
	m_start_info.hStdOutput	= m_org_std_output;
	m_start_info.hStdError	= m_org_std_error;
	m_start_info.dwFlags	|= STARTF_USESTDHANDLES;

	m_is_created			= false;
	m_is_input_redirect		= false;
	m_is_output_redirect	= false;
	m_is_error_redirect		= false;
}

CProcBase::~CProcBase()
{
	terminate();
	::CloseHandle(m_proc_info.hThread);
	::CloseHandle(m_proc_info.hProcess);
	m_proc_info.hThread = NULL;
	m_proc_info.hProcess = NULL;
}

bool CProcBase::create(const char* sCmdLine, const char* sWorkPath/* = NULL*/)
{
	char sDCmdLine[1024];
	strcpy(sDCmdLine, sCmdLine);

	bool b_ret = !!::CreateProcess(
		NULL,
		sDCmdLine,
		NULL,
		NULL,
		TRUE,
#ifdef _DEBUG
		CREATE_NEW_CONSOLE | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB,
#else
		CREATE_NO_WINDOW | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB,
#endif
		NULL,
		sWorkPath,
		&m_start_info,
		&m_proc_info);
	
	if(!b_ret)
		return false;

	// 句柄已经被继承，无用
	resetInputDevice();
	resetOutputDevice();
	resetErrorDevice();

	m_is_created = true;
	return b_ret;
}

int CProcBase::run(long nMilliSeconds /* = INFTIME */)
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
	return -1;
}

int CProcBase::getExitCode()
{
	DWORD _exit_code = 0;
	::GetExitCodeProcess(m_proc_info.hProcess, &_exit_code);
	return _exit_code;
}

bool CProcBase::terminate()
{
	::TerminateProcess(m_proc_info.hProcess, 1);

	return true;
}

bool CProcBase::setInputDevice(DeviceHandleT& hDevice)
{
	if (hDevice == INVALID_HANDLE_VALUE)
		return false;
	resetInputDevice();
	m_start_info.hStdInput = hDevice;
	m_is_input_redirect = true;
	return true;
}

bool CProcBase::setOutputDevice(DeviceHandleT& hDevice)
{
	if (hDevice == INVALID_HANDLE_VALUE)
		return false;
	resetOutputDevice();
	m_start_info.hStdOutput = hDevice;
	m_is_output_redirect = true;
	return true;
}

bool CProcBase::setErrorDevice(DeviceHandleT& hDevice)
{
	if (hDevice == INVALID_HANDLE_VALUE)
		return false;
	resetErrorDevice();
	m_start_info.hStdError = hDevice;
	m_is_error_redirect = true;
	return true;
}

void CProcBase::resetInputDevice()
{
	if (m_is_input_redirect)
		::CloseHandle(m_start_info.hStdInput);

	m_is_input_redirect = false;
	m_start_info.hStdInput = m_org_std_input;
}

void CProcBase::resetOutputDevice()
{
	if (m_is_output_redirect)
		::CloseHandle(m_start_info.hStdOutput);

	m_is_output_redirect = false;
	m_start_info.hStdOutput = m_org_std_output;
}

void CProcBase::resetErrorDevice()
{
	if (m_is_error_redirect)
		::CloseHandle(m_start_info.hStdError);

	m_is_error_redirect = false;
	m_start_info.hStdError = m_org_std_error;
}

bool CProcBase::setInputFile(const char* sFilePath)
{
	if (sFilePath == NULL)
		return false;

	resetInputDevice();

	SECURITY_ATTRIBUTES saAttr = {0};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	PipeHandleT hFile = CreateFile(
		sFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		&saAttr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return setInputDevice(hFile);
}

bool CProcBase::setOutputFile(const char* sFilePath)
{
	if (sFilePath == NULL)
		return false;

	resetOutputDevice();

	SECURITY_ATTRIBUTES saAttr = {0};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	PipeHandleT hFile = CreateFile(
		sFilePath,
		GENERIC_WRITE,
		NULL,
		&saAttr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return setOutputDevice(hFile);
}

bool CProcBase::setErrorFile(const char* sFilePath/* = NULL*/)
{
	if (sFilePath == NULL)
		return false;

	resetErrorDevice();

	SECURITY_ATTRIBUTES saAttr = {0};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	PipeHandleT hFile = CreateFile(
		sFilePath,
		GENERIC_WRITE,
		NULL,
		&saAttr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return setErrorDevice(hFile);
}

int CProcBase::getRunTime()
{
	return BaseFunc::getProcessTime(m_proc_info.hProcess);
}

int CProcBase::getRunMemory()
{
	return BaseFunc::getProcessMemory(m_proc_info.hProcess);
}

ProcessHandleT CProcBase::getProcessHandle()
{
	return m_proc_info.hProcess;
}

ProcessHandleT CProcBase::getThreadHandle()
{
	return m_proc_info.hThread;
}




////////////////////////////////////////////
bool CPipeProc::initPipe()
{
	m_read_pipe.init();
	m_write_pipe.init();
	return true;
}

int CPipeProc::send(const char* sBuff, int nBuffSize)
{
	return m_write_pipe.send(sBuff, nBuffSize);
}

int CPipeProc::recv(char* sBuff, int nBuffSize)
{
	return m_read_pipe.recv(sBuff, nBuffSize);
}

bool CPipeProc::setInputPipe()
{
	m_process.resetInputDevice();
	CPipeObject& write_pipe = m_write_pipe.getPipe();
	bool b_ret = m_process.setInputDevice(write_pipe.getReadPort());
	// 这个句柄交给进程处理了，自己不处理，但是不能删除
	if (b_ret) write_pipe.m_read_port = NULL;
	return b_ret;
}

bool CPipeProc::setOutputPipe()
{
	m_process.resetOutputDevice();
	CPipeObject& read_pipe = m_read_pipe.getPipe();
	bool b_ret = m_process.setOutputDevice(read_pipe.getWritePort());
	// 这个句柄交给进程处理了，自己不处理，但是不能删除
	if (b_ret) read_pipe.m_write_port = NULL;
	return b_ret;
}

bool CPipeProc::setErrorPipe()
{
	m_process.resetErrorDevice();
	return m_process.setErrorDevice(m_read_pipe.getPipe().getWritePort());
}