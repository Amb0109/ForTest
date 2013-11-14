#include "stdafx.h"
#include "JsonObject.h"

#include "BaseFunc.h"

bool CJsonOperator::readFromFile(CJsonValue& cJsonValue, const char* sFileName)
{
	cJsonValue = CJsonValue::null;

	std::string f_content;
	int _ret = BaseFunc::getFileContent(f_content, sFileName);
	if (_ret == -1) return false;
	return readFromString(cJsonValue, f_content);
}

bool CJsonOperator::readFromString(CJsonValue& cJsonValue, const char* sJsonStr)
{
	return readFromString(cJsonValue, std::string(sJsonStr));
}

bool CJsonOperator::readFromString(CJsonValue& cJsonValue, std::string sJsonStr)
{
	Json::Reader json_reader;
	cJsonValue = CJsonValue::null;
	return json_reader.parse(sJsonStr, cJsonValue);
}

bool CJsonOperator::writeToString(char* sJsonStr, unsigned nStrSize, CJsonValue& cJsonValue, bool noStyle/*= true*/)
{
	if(nStrSize == 0) return true;

	std::string json_tmp;
	bool b_ret = writeToString(json_tmp, cJsonValue, noStyle);
	
	if (nStrSize > json_tmp.length())
	{
		strcpy(sJsonStr, json_tmp.c_str());
	}
	else
	{
		strncpy(sJsonStr, json_tmp.c_str(), nStrSize);
	}
	return b_ret;
}

bool CJsonOperator::writeToString(std::string& sJsonStr, CJsonValue& cJsonValue, bool noStyle/*= true*/)
{
	if (noStyle)
	{
		Json::FastWriter json_write;
		sJsonStr = json_write.write(cJsonValue);
		if (sJsonStr.length() > 0)
			sJsonStr = sJsonStr.substr(0, sJsonStr.length()-1);
	}
	else
	{
		Json::StyledWriter json_write;
		sJsonStr = json_write.write(cJsonValue);
	}
	return true;
}

bool CJsonOperator::writeToFile(const char* sFileName, CJsonValue& cJsonValue, bool noStyle/*= false*/)
{
	FILE* p_file = NULL;
	p_file = fopen(sFileName, "w");
	if (p_file == NULL)
		return false;

	std::string json_buf;
	writeToString(json_buf, cJsonValue, noStyle);

	fwrite(json_buf.c_str(), json_buf.size(), 1, p_file);
	fclose(p_file);
	return true;
}

