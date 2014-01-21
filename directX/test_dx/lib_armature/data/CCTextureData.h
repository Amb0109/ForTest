#ifndef _CC_TEXTURE_DATA_H_
#define _CC_TEXTURE_DATA_H_

#include "CCData.h"
#include "../cocos/CCMacro.h"

namespace CC {

struct CC_DLL CCContourVetrex
{
	float x;
	float y;
};

class CC_DLL CCContourData
{
public:
	bool			load(Json::Value& json, CCDataCollection* data);

public:
	std::vector<CCContourVetrex> m_vecVertiesList;
};

class CC_DLL CCTextureData
{
public:
	bool			load(Json::Value& json, CCDataCollection* data);

	std::string&	getName() { return m_sName; }
	CCContourData*	getContourData(int index = 0);

	MEMBER_GET_PTR(CCDataCollection*, m_pDataCollection, DataCollection);
public:
	std::string					m_sName;
	float						m_fWidth;
	float						m_fHeight;
	float						m_fPivotX;
	float						m_fPivotY;
	//std::string				m_sPlistFile;
	std::vector<CCContourData>	m_vecContourData;
};

} // namespace

#endif //_CC_TEXTURE_DATA_H_