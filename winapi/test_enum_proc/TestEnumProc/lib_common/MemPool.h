#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "stdafx.h"
#include "SafeLock.h"

template <typename BLOCK_TYPE>
class CMemPool
{
public:
	explicit CMemPool(unsigned PageSize);
	~CMemPool() { clearMemPool(); }

public:
	bool			newPage(int PageCnt = 1);
	void			clearMemPool();

	BLOCK_TYPE*		allocBlock();
	bool			freeBlock(BLOCK_TYPE* pBlock);

	bool			addRef(BLOCK_TYPE* pBlock);
	bool			delRef(BLOCK_TYPE* pBlock);

private:
	typedef std::vector<BLOCK_TYPE*>	PAGE_PTR;
	typedef std::map<BLOCK_TYPE*, int>	REF_CNT_MAP;

	int				m_page_size;

	int				m_total_block;
	int				m_free_block;

	REF_CNT_MAP		m_ref_map;
	PAGE_PTR		m_pages;
	CSafeLock		m_lock;
};

template <typename BLOCK_TYPE>
CMemPool<BLOCK_TYPE>::CMemPool(unsigned PageSize)
{
	m_ref_map.clear();
	m_pages.clear();
	m_page_size		= PageSize;
	m_total_block	= 0;
	m_free_block	= 0;
}

template <typename BLOCK_TYPE>
bool CMemPool<BLOCK_TYPE>::newPage(int PageCnt/* = 1*/)
{
	CAutoLock _LOCK(&m_lock);
	
	if (m_page_size<=0) return false;

	for (int i=0; i<PageCnt; i++)
	{
		BLOCK_TYPE* new_page = new BLOCK_TYPE[m_page_size];
		
		for (int j=0; j<m_page_size; j++)
			m_ref_map[&(new_page[j])] = 0;

		m_pages.push_back(new_page);
	}

	m_total_block	+= PageCnt * m_page_size;
	m_free_block	+= PageCnt * m_page_size;

	return true;
}

template <typename BLOCK_TYPE>
void CMemPool<BLOCK_TYPE>::clearMemPool()
{
	CAutoLock _LOCK(&m_lock);

	for_std_unit(PAGE_PTR, m_pages, ptr_page)
		delete[] *ptr_page;
	
	m_pages.clear();
	m_ref_map.clear();
	m_total_block	= 0;
	m_free_block	= 0;
}

template <typename BLOCK_TYPE>
BLOCK_TYPE* CMemPool<BLOCK_TYPE>::allocBlock()
{
	CAutoLock _LOCK(&m_lock);

	if (m_page_size <= 0) return NULL;

	if(m_free_block == 0)
		newPage();

	BLOCK_TYPE* ptr_block = NULL;
	for_std_unit (REF_CNT_MAP, m_ref_map, it_ref)
	{
		if(0 == it_ref->second)
		{
			ptr_block = it_ref->first;
			break;
		}
	}

	assert(ptr_block != NULL);
	if (ptr_block == NULL) return NULL;
	if( !addRef(ptr_block) ) return NULL;
	return ptr_block;
}

template <typename BLOCK_TYPE>
bool CMemPool<BLOCK_TYPE>::freeBlock(BLOCK_TYPE* pBlock)
{
	return delRef(pBlock);
}


template <typename BLOCK_TYPE>
bool CMemPool<BLOCK_TYPE>::addRef( BLOCK_TYPE* pBlock )
{
	CAutoLock _LOCK(&m_lock);
	
	REF_CNT_MAP::iterator it_ref = m_ref_map.find(pBlock);
	if (it_ref == m_ref_map.end())
		return false;

	if (it_ref->second == 0)
		m_free_block --;

	it_ref->second++;
	return true;
}

template <typename BLOCK_TYPE>
bool CMemPool<BLOCK_TYPE>::delRef( BLOCK_TYPE* pBlock )
{
	CAutoLock _LOCK(&m_lock);

	REF_CNT_MAP::iterator it_ref = m_ref_map.find(pBlock);
	if (it_ref == m_ref_map.end())
		return false;

	if ( it_ref->second <= 0)
		return false;

	it_ref->second --;
	if (it_ref->second == 0)
		m_free_block ++;

	return true;
}

#endif // _MEMORY_POOL_H_