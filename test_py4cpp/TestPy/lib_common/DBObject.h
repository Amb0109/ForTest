#ifndef _DATABASE_OBJECT_H_
#define _DATABASE_OBJECT_H_

#include "stdafx.h"
#include "mysql.h"
#include "SafeLock.h"

typedef struct _DATABASE_KEY
{
	std::string		sServer;
	std::string		sUser;
	std::string		sPassword;
	std::string		sDBName;
	int				nPort;
} DatabaseKey;

class CDBObject
{
public:
	typedef MYSQL_RES* PQUERY_RESULT;

public:
	CDBObject();
	~CDBObject();

public:
	bool InitLibrary();
	bool ConnectDB(const DatabaseKey* DBPass);
	void CloseDB();

	const char*		Error();

	bool			Query(const char* sSQLQuery);
	PQUERY_RESULT	StoreResult();
	int				NextResult();
	void			FreeResult(PQUERY_RESULT& pQueryRes);
	bool			ClearResult();
	char**			FetchRow(const PQUERY_RESULT& pQueryRes);
	
	int				getEscapeString(char* sDstStr, const char* sSrcStr, size_t nSrcLen);

private:
	MYSQL			m_mysql;
};

#endif // _DATABASE_OBJECT_H_