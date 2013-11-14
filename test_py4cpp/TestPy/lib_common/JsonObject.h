#ifndef _JSON_OBJECT_H_
#define _JSON_OBJECT_H_

#include "stdafx.h"
#include "json.h"

typedef Json::Value CJsonValue;

class CJsonOperator
{
public:
	static bool	readFromFile(CJsonValue& cJsonValue, const char* sFileName);
	static bool	readFromString(CJsonValue& cJsonValue, const char* sJsonStr);
	static bool	readFromString(CJsonValue& cJsonValue, std::string sJsonStr);

	static bool	writeToString(char* sJsonStr, unsigned nStrSize, CJsonValue& cJsonValue, bool noStyle = true);
	static bool	writeToString(std::string& sJsonStr, CJsonValue& cJsonValue, bool noStyle = true);
	static bool	writeToFile(const char* sFileName, CJsonValue& cJsonValue, bool noStyle = false);
};


#endif // _JSON_OBJECT_H_