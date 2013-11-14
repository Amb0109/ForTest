#include "stdafx.h"
#include "LockedFolder.h"
#include "BaseFunc.h"



CLockedFolder::CLockedFolder()
{

}

CLockedFolder::~CLockedFolder()
{
	unlock();
}

CLockedFolder::CLockedFolder(const char* dir_path)
{
	setFolder(dir_path);
}

void CLockedFolder::setFolder( const char* dir_path )
{
	m_dir_path = dir_path;
}


std::string CLockedFolder::getFolder()
{
	return m_dir_path;
}

bool CLockedFolder::lock()
{
	return true;
}

void CLockedFolder::unlock()
{
}

bool CLockedFolder::clear()
{
	BaseFunc::clearDirectory(m_dir_path.c_str());
	return true;
}

bool CLockedFolder::create()
{
	return BaseFunc::newDirectory(m_dir_path.c_str());
}


