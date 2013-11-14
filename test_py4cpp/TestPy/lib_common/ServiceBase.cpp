#include "stdafx.h"
#include "ServiceBase.h"


ServiceBase::ServiceBase()
{
}

ServiceBase::~ServiceBase()
{

}

ServiceBase* ServiceBase::getInstance()
{
	static ServiceBase gGameServer;
	return &gGameServer;
}

bool ServiceBase::initService(const char* sServiceName)
{
	m_service_name = sServiceName;
	m_table_entry[0].lpServiceName = const_cast<char*>(m_service_name.c_str());
	m_table_entry[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	m_table_entry[1].lpServiceName = NULL;
	m_table_entry[1].lpServiceProc = NULL;

	bool b_ret = true;
	b_ret = b_ret && ::StartServiceCtrlDispatcher(m_table_entry);
	return b_ret;
}

bool ServiceBase::startService()
{
	m_status.dwServiceType				= SERVICE_WIN32; 
	m_status.dwCurrentState				= SERVICE_START_PENDING; 
	m_status.dwControlsAccepted			= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	m_status.dwWin32ExitCode			= 0;
	m_status.dwServiceSpecificExitCode	= 0;
	m_status.dwCheckPoint				= 0;
	m_status.dwWaitHint					= 0;

	m_status_hnd = ::RegisterServiceCtrlHandler(
		m_service_name.c_str(),
		(LPHANDLER_FUNCTION)ServiceBase::CtrlHandler);

	if (m_status_hnd == NULL)
		return false;

	if((*m_func_init)())
	{
		m_status.dwCurrentState			= SERVICE_RUNNING; 
		m_status.dwCheckPoint			= 0;
		m_status.dwWaitHint				= 0;  
		::SetServiceStatus(m_status_hnd, &m_status);
		return true;
	}
	else
	{
		m_status.dwCurrentState				= SERVICE_STOPPED;
		m_status.dwWaitHint					= 0;
		m_status.dwServiceSpecificExitCode	= 1;
		m_status.dwWin32ExitCode			= GetLastError();
		::SetServiceStatus(m_status_hnd, &m_status);
		return false;
	}
	return true;
}

void ServiceBase::stopService()
{
	m_status.dwWin32ExitCode	= -1; 
	m_status.dwCurrentState		= SERVICE_STOP_PENDING;
	m_status.dwCurrentState		= SERVICE_STOPPED;
}

void ServiceBase::servControl(unsigned nRequest)
{
	switch(nRequest)
	{
	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_ACCEPT_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		m_status.dwWin32ExitCode		= 0; 
		m_status.dwCurrentState			= SERVICE_STOP_PENDING;
		(*m_func_stop)();
		m_status.dwWin32ExitCode		= 0; 
		m_status.dwCurrentState			= SERVICE_STOPPED;
		break;
	} 

	::SetServiceStatus(m_status_hnd, &m_status);
}

void ServiceBase::setServerInitFunc( InitServerFunc fInitServer )
{
	m_func_init = fInitServer;
}

void ServiceBase::setServerStopFunc( StopServerFunc fStopServer )
{
	m_func_stop = fStopServer;
}

void WINAPI ServiceBase::ServiceMain(int argc, char** argv)
{
	ServiceBase* p_service = ServiceBase::getInstance();
	p_service->startService();
}

void WINAPI ServiceBase::CtrlHandler( unsigned nRequest )
{
	ServiceBase* p_service = ServiceBase::getInstance();
	p_service->servControl(nRequest);
}
