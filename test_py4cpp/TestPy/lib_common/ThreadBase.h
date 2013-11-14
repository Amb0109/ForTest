#ifndef _THREAD_BASE_H_
#define _THREAD_BASE_H_

#include "stdafx.h"
#include "SafeLock.h"

class CThreadBase
{
public:
	enum {
		TPL_TIME_CRITICAL = 15,
		TPL_HIGHEST = 2,
		TPL_ABOVE_NORMAL = 1,
		TPL_NORMAL = 0,
		TPL_BELOW_NORMAL = -1,
		TPL_LOWEST = -2,
		TPL_IDLE = -15
	};

public:
	CThreadBase();
	~CThreadBase();

public:
	virtual bool Create();
	virtual void Close(long nMilliSeconds = 0);
	
	virtual bool setThreadPriority(int nLevel);

protected:
	virtual void onInit() {}
	virtual bool onProcess() = 0;
	virtual void onDestory() { ::SetEvent(m_exit_event); }

protected:
	virtual unsigned ThreadProc();

	static unsigned WINAPI ThreadMain(void* Param);

protected:
	bool			m_is_exit;

	ThreadHandleT		m_thread;
	EventHandleT			m_exit_event;
};

template<typename TASK_TYPE>
class CTaskThread : public CThreadBase
{
public:
	CTaskThread() { m_idle_event = NULL; }
	~CTaskThread(){}

public:
	virtual bool Create();
	virtual void Close(long nMilliSeconds = 0);

	virtual void addTask(TASK_TYPE aTask);
	virtual bool getTask(TASK_TYPE& aTask);
	virtual bool popTask();
	virtual bool isEmpty();

protected:
	virtual bool onProcess();
	virtual bool procTask(TASK_TYPE aTask) = 0;

	virtual unsigned ThreadProc();

private:
	EventHandleT					m_idle_event;
	
	std::queue<TASK_TYPE>	m_task_queue;
	CSafeLock				m_lock;
};


template<typename TASK_TYPE>
bool CTaskThread<TASK_TYPE>::isEmpty()
{
	CAutoLock _LOCK(&m_lock);
	return m_task_queue.empty();
}


// 模板类不支持分离编译
template<typename TASK_TYPE>
bool CTaskThread<TASK_TYPE>::Create()
{
	CAutoLock _LOCK(&m_lock);
	m_is_exit = false;
	if (m_idle_event == NULL)
	{
		m_idle_event = ::CreateEvent(NULL, TRUE, TRUE, NULL);
		if (m_idle_event == NULL)
			return false;
	}

	if (!CThreadBase::Create())
	{
		::CloseHandle(m_idle_event);
		m_idle_event = NULL;
		return false;
	}

	return true;
}

template<typename TASK_TYPE>
void CTaskThread<TASK_TYPE>::Close(long nMilliSeconds/* = 0 */)
{
	m_lock.Lock();
	m_is_exit = true;
	::SetEvent(m_idle_event);
	m_lock.unLock();
	CThreadBase::Close(nMilliSeconds);
}

template<typename TASK_TYPE>
void CTaskThread<TASK_TYPE>::addTask(TASK_TYPE aTask)
{
	CAutoLock _LOCK(&m_lock);
	m_task_queue.push(aTask);
	::SetEvent(m_idle_event);
}

template<typename TASK_TYPE>
bool CTaskThread<TASK_TYPE>::getTask(TASK_TYPE& aTask)
{
	CAutoLock _LOCK(&m_lock);
	if (isEmpty())
		return false;
	aTask = m_task_queue.front();
	return true;
}

template<typename TASK_TYPE>
bool CTaskThread<TASK_TYPE>::popTask()
{
	CAutoLock _LOCK(&m_lock);
	if (isEmpty())
		return false;
	m_task_queue.pop();
	return true;
}

template<typename TASK_TYPE>
bool CTaskThread<TASK_TYPE>::onProcess()
{
	TASK_TYPE the_task = NULL;
	if(getTask(the_task))
		return procTask(the_task);
	return true;
}

template<typename TASK_TYPE>
unsigned CTaskThread<TASK_TYPE>::ThreadProc()
{
	unsigned b_ret = 0;
	onInit();
	while (!m_is_exit)
	{
		::WaitForSingleObject(m_idle_event, INFINITE);

		{
			CAutoLock _LOCK(&m_lock);
			if (m_is_exit)
				break;

			if (m_task_queue.empty())
			{
				::ResetEvent(m_idle_event);
				continue;
			}
		}

		if(!onProcess())
		{
			b_ret = 1;
			break;
		}
	}
	onDestory();
	return b_ret;
}

#endif // _THREAD_BASE_H_