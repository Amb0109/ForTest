#include "stdafx.h"
#include "ServiceCtrl.h"
#include "BaseFunc.h"

ServiceCtrl::ServiceCtrl():
c_try_cnt(20)
{
	m_sc_manager = NULL;
	m_sc_service = NULL;
}

ServiceCtrl::~ServiceCtrl()
{
	::CloseServiceHandle(m_sc_manager);
}

bool ServiceCtrl::OpenManager()
{
	m_sc_manager = ::OpenSCManager( 
		NULL,
		NULL,
		SC_MANAGER_ALL_ACCESS);

	if (NULL == m_sc_manager)
		return false;

	return true;
}

bool ServiceCtrl::installService(const char* sServiceName, const char* sSvrDisplayName, const char* sServicePath)
{
	if (m_sc_manager == NULL)
		return false;

	m_sc_service = ::OpenService( 
		m_sc_manager,
		sServiceName,
		READ_CONTROL);

	if (m_sc_service != NULL)
	{
		::CloseServiceHandle(m_sc_service);
		m_sc_service = NULL;
		return false;
	}

	m_sc_service = ::CreateService(
		m_sc_manager,
		sServiceName,
		sSvrDisplayName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		sServicePath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if (m_sc_service == NULL)
		return false;
	else
	{
		::CloseServiceHandle(m_sc_service);
		m_sc_service = NULL;
		return true;
	}
}

bool ServiceCtrl::removeService( const char* sServiceName )
{
	if (m_sc_manager == NULL)
		return false;

	m_sc_service = ::OpenService( 
		m_sc_manager,
		sServiceName,
		SERVICE_STOP|DELETE);

	if(m_sc_service == NULL)
		return false;
	
	SERVICE_STATUS service_status;
	::ControlService(m_sc_service, SERVICE_CONTROL_STOP, &service_status);

	bool b_ret = !!::DeleteService(m_sc_service);
	::CloseServiceHandle(m_sc_service);
	m_sc_service = NULL;
	return b_ret;
}

bool ServiceCtrl::startService( const char* sServiceName )
{
	if (m_sc_manager == NULL)
		return false;

	m_sc_service = ::OpenService( 
		m_sc_manager, sServiceName, SERVICE_ALL_ACCESS); 

	if (m_sc_service == NULL) 
		return false;

	bool b_ret = true;
	b_ret = b_ret && ::StartService(m_sc_service, 0, NULL);
	if (!b_ret)
	{
		::CloseServiceHandle(m_sc_service);
		m_sc_service = NULL;
		return false;
	}

	SERVICE_STATUS service_status;
	for (int i=0; i < c_try_cnt; ++i)
	{
		::QueryServiceStatus(m_sc_service, &service_status);
		if (service_status.dwCurrentState == SERVICE_RUNNING)
			break;
		BaseFunc::sleep(1000);
	}

	::CloseServiceHandle(m_sc_service);
	m_sc_service = NULL;
	b_ret = b_ret && (service_status.dwCurrentState == SERVICE_RUNNING);
	return b_ret;
}

bool ServiceCtrl::stopService(const char* sServiceName)
{
	if (m_sc_manager == NULL)
		return false;

	m_sc_service = ::OpenService( 
		m_sc_manager, sServiceName, SERVICE_ALL_ACCESS);

	if (m_sc_service == NULL)
		return false;

	bool b_ret = true;
	SERVICE_STATUS service_status;
	b_ret = b_ret && ::ControlService(m_sc_service, SERVICE_CONTROL_STOP, &service_status);

	if (!b_ret)
	{
		::CloseServiceHandle(m_sc_service);
		m_sc_service = NULL;
		return b_ret;
	}

	for (int i=0; i < c_try_cnt; ++i)
	{
		::QueryServiceStatus(m_sc_service, &service_status);
		if (service_status.dwCurrentState == SERVICE_STOPPED)
			break;
		BaseFunc::sleep(1000);
	}

	::CloseServiceHandle(m_sc_service);
	m_sc_service = NULL;
	return b_ret;
}

bool ServiceCtrl::ctrlService(const char* sServiceName, unsigned nCtrl)
{
	if (m_sc_manager == NULL)
		return false;

	m_sc_service = ::OpenService( 
		m_sc_manager, sServiceName, SERVICE_ALL_ACCESS);

	if (m_sc_service == NULL)
		return false;

	bool b_ret = true;
	SERVICE_STATUS service_status;
	b_ret = b_ret && ::ControlService(m_sc_service, nCtrl, &service_status);
	::CloseServiceHandle(m_sc_service);
	m_sc_service = NULL;
	return b_ret;
}
