#ifndef _CC_ARMATURE_DATA_H_
#define _CC_ARMATURE_DATA_H_

#include "../cocos/CCMacro.h"
#include "CCData.h"

namespace CC {

enum CC_DLL CCDisplayType
{
	CC_DISPLAY_SPRITE = 0,       //! display is a single CCSprite
	CC_DISPLAY_ARMATURE,         //! display is a CCArmature
	CC_DISPLAY_PARTICLE,         //! display is a CCParticle.

	CC_DISPLAY_MAX
};

class CC_DLL CCDisplayData
{
public:
	bool			load(Json::Value& json, CCDataCollection* data);

	std::string&	getName() { return m_sName; }
	CCBaseData*		getSkinData(int index = 0);

public:
	std::string					m_sName;
//	std::string					m_sPlist;
	int							m_eDisplayType;
	std::vector<CCBaseData>		m_vecSkinData;
};

class CC_DLL CCBoneData : public CCBaseData
{
public:
	CCBoneData();
	virtual ~CCBoneData();

public:
	bool			load(Json::Value& json, CCDataCollection* data);

	std::string&	getName() { return m_sName; }
	CCDisplayData*	getDisplayData(int index = 0);
	CCDisplayData*	getDisplayDataByName(const char* name);

public:
	std::string					m_sName;
	std::string					m_sParent;
//	float						m_fArrowX;
//	float						m_fArrowY;
//	bool						m_bEffectBySkeleton;
	std::vector<CCDisplayData>	m_vecDisplayDataList;
	DATA_NAME_MAP				m_mapDisplayDataMap;
};

class  CC_DLL CCArmatureData
{
public:
	bool			load(Json::Value& json, CCDataCollection* data);

	std::string&	getName() { return m_sName; }
	CCBoneData*		getBoneData(int index = 0);
	CCBoneData*		getBoneDataByName(const char* name);

	MEMBER_GET_PTR(CCDataCollection*, m_pDataCollection, DataCollection);
public:
	std::string					m_sName;
	float						m_fVersion;
	std::vector<CCBoneData>		m_vecBoneDataList;
	DATA_NAME_MAP				m_mapBoneDataMap;
};

} // namespace

#endif //_CC_ARMATURE_DATA_H_
