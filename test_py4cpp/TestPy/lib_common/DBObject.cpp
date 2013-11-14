#include "stdafx.h"
#include "DBObject.h"

CDBObject::CDBObject()
{
}

CDBObject::~CDBObject()
{
	CloseDB();
}

bool CDBObject::InitLibrary()
{
	if(mysql_library_init(0, NULL, NULL))
		return false;
	return true;
}

bool CDBObject::ConnectDB(const DatabaseKey* DBKey)
{
//	bool b_ret = false;

	if (!mysql_init( &m_mysql ))
		return false;

	if (mysql_real_connect(
		&m_mysql,
		DBKey->sServer.c_str(),
		DBKey->sUser.c_str(),
		DBKey->sPassword.c_str(),
		DBKey->sDBName.c_str(),
		DBKey->nPort,
		NULL, // UNIX SOCKET
		CLIENT_MULTI_STATEMENTS) == NULL)
	{
		mysql_close( &m_mysql );
		return false;
	}

	return true;
}

void CDBObject::CloseDB()
{
	mysql_close(&m_mysql);
}

const char* CDBObject::Error()
{
	return mysql_error(&m_mysql);
}

bool CDBObject::Query(const char* sSQLQuery)
{
	return 0 == mysql_query(&m_mysql, sSQLQuery);
}

CDBObject::PQUERY_RESULT CDBObject::StoreResult()
{
	return mysql_store_result(&m_mysql);
}

int CDBObject::NextResult()
{
	return mysql_next_result(&m_mysql);
}

void CDBObject::FreeResult( PQUERY_RESULT& pQueryRes )
{
	mysql_free_result(pQueryRes);
	pQueryRes = NULL;
}


bool CDBObject::ClearResult()
{
	while(true) {
		int t_ret = NextResult();
		if( t_ret < 0) {
			return true;
		} else if (t_ret > 0) {
			return false;
		}
		PQUERY_RESULT tmp_res = StoreResult();
		FreeResult(tmp_res);
	}
	return true;
}

char** CDBObject::FetchRow(const PQUERY_RESULT& pQueryRes)
{
	return (char**)mysql_fetch_row(pQueryRes);
}

int CDBObject::getEscapeString( char* sDstStr, const char* sSrcStr, size_t nSrcLen )
{
	int n_len = mysql_real_escape_string(&m_mysql, sDstStr, sSrcStr, (unsigned long)nSrcLen);
	return n_len;
}

//int CDBObject::getEscapeString(std::string& sDstStr, const char* sSrcStr, int nSrcLen)
//{
//	char _buff[1024*2+1];
//	char* _p_src = sSrcStr;
//	int _len = nSrcLen;
//
//	sDstStr.clear();
//	while (_len > 0)
//	{
//		if (_len >= 1024)
//		{
//			int _ret_len = mysql_real_escape_string(&m_mysql, _buff, _p_src, 1024);
//			_buff[_ret_len] = '\0';
//			sDstStr.append(_buff);
//			_p_src += 1024;
//			_len -= 1024;
//		} else {
//			int _ret_len = mysql_real_escape_string(&m_mysql, _buff, _p_src, _len);
//			_buff[_ret_len] = '\0';
//			sDstStr.append(_buff);
//			_p_src += _len;
//			_len -= _len;
//		}
//	}
//	return sDstStr.size();
//}
