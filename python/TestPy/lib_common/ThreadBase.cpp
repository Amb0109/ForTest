#include "stdafx.h"
#include "ThreadBase.h"

CThreadBase::CThreadBase()
{
	m_thread = NULL;
	m_exit_event = NULL;
	m_is_exit = false;
}

CThreadBase::~CThreadBase()
{
	if (m_thread != NULL)
		Close();
}

bool CThreadBase::Create()
{
	if (m_thread != NULL)
		return true;

	m_is_exit = false;
	m_exit_event	= ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_thread		= ::CreateThread(NULL, 0,
		(DWORD (WINAPI *)(LPVOID))ThreadMain, (void*)this,
		NULL, NULL);

	if (m_thread == NULL || m_exit_event == NULL)
		return false;
	else
		return true;
}

void CThreadBase::Close(long nMilliSeconds/* = 0 */)
{
	m_is_exit = true;
	if( WAIT_TIMEOUT == ::WaitForSingleObject(m_exit_event, nMilliSeconds))
	{
		::TerminateThread(m_thread, 1);
	}
	::CloseHandle(m_thread);
	m_thread = NULL;
}

unsigned CThreadBase::ThreadProc()
{
	unsigned b_ret = 0;
	onInit();
	while (!m_is_exit)
	{
		if(!onProcess())
		{
			b_ret = 1;
			break;
		}
	}
	onDestory();
	return b_ret;
}

unsigned WINAPI CThreadBase::ThreadMain(void* Param)
{
	CThreadBase* pThread;
	pThread	= static_cast<CThreadBase*>(Param);
	return pThread->ThreadProc();
}

bool CThreadBase::setThreadPriority( int nLevel )
{
	return !!::SetThreadPriority(m_thread, nLevel);
}

