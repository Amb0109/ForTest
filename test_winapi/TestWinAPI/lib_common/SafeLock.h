#ifndef _SAFE_LOCK_H_
#define _SAFE_LOCK_H_

#include "stdafx.h"


class CSafeLock
{
public:
	CSafeLock() {InitializeCriticalSection( &m_lock );}
	~CSafeLock() {DeleteCriticalSection( &m_lock );}

public:
	bool tryEnter() {TryEnterCriticalSection( &m_lock );}
	void Lock() {EnterCriticalSection( &m_lock );}
	void unLock() {LeaveCriticalSection( &m_lock );}

private:
	CSMutexT m_lock;
};

class CAutoLock
{
public:
	explicit CAutoLock(CSafeLock* safe_lock)
	{
		p_safe_lock = safe_lock;
		if (p_safe_lock)
			p_safe_lock->Lock();
	}
	~CAutoLock()
	{
		if (p_safe_lock)
			p_safe_lock->unLock();
	}

private:
	CSafeLock*		p_safe_lock;
};

template<typename ValType>
class CSafeQueue
{
public:
	CSafeQueue() {};
	~CSafeQueue() {};

public:
	void Push(const ValType& ValBody)
	{
		CAutoLock _LOCK(&m_lock);
		m_queue.push(ValBody);
	}

	bool Pop(ValType& ValBody)
	{
		CAutoLock _LOCK(&m_lock);
		if (Empty())
			return false;

		ValBody = m_queue.front();
		m_queue.pop();
		return true;
	}

	bool Empty()
	{
		CAutoLock _LOCK(&m_lock);
		return m_queue.empty();;
	}

	int Size()
	{
		CAutoLock _LOCK(&m_lock);
		return m_queue.size();
	}

private:
	std::queue<ValType>		m_queue;
	CSafeLock				m_lock;
};


template<typename KeyType, typename ValType>
class CSafeMap
{
public:
	CSafeMap() {};
	~CSafeMap() {};

public:
	void Set(const KeyType& KeyBody, const ValType& ValBody)
	{
		CAutoLock _LOCK(&m_lock);
		m_map[KeyBody] = ValBody;
	}

	bool Get(const KeyType& KeyBody, ValType& ValBody)
	{
		CAutoLock _LOCK(&m_lock);
		std::map<KeyType, ValType>::iterator it;
		it = m_map.find(KeyBody);
		if (it == m_map.end())
			return false;

		ValBody = *it;
		return true;
	}

	void clear()
	{
		CAutoLock _LOCK(&m_lock);
		m_map.clear();
	}

private:
	std::map<KeyType, ValType>	m_map;
	CSafeLock					m_lock;
};


#endif // _SAFE_LOCK_H_