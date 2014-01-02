#ifndef _CC_ARMATURE_DATA_H_
#define _CC_ARMATURE_DATA_H_

#include "CCData.h"

enum CCDisplayType
{
	CC_DISPLAY_SPRITE = 0,       //! display is a single CCSprite
	CC_DISPLAY_ARMATURE,         //! display is a CCArmature
	CC_DISPLAY_PARTICLE,         //! display is a CCParticle.

	CC_DISPLAY_MAX
};

class CCDisplayData
{
public:
	bool			load(Json::Value& json, CCData* data);

	std::string&	get_name() { return m_name; }
	CCBaseData*		get_skin_data(int index = 0);

public:
	std::string					m_name;
	std::string					m_plist;
	int							m_displayType;
	std::vector<CCBaseData>		m_skin_data;
};

class CCBoneData : public CCBaseData
{
public:
	bool			load(Json::Value& json, CCData* data);

	std::string&	get_name() { return m_name; }
	CCDisplayData*	get_display_data(int index = 0);
	CCDisplayData*	get_display_data_by_name(const char* name);

public:
	std::string					m_name;
	std::string					m_parent;
	//float						m_arrow_x;
	//float						m_arrow_y;
	//bool						m_effectbyskeleton;
	std::vector<CCDisplayData>	m_display_data;
	DATA_NAME_MAP	m_display_data_map;
};

class CCArmatureData
{
public:
	bool			load(Json::Value& json, CCData* data);

	std::string&	get_name() { return m_name; }
	CCBoneData*		get_bone_data(int index = 0);
	CCBoneData*		get_bone_data_by_name(const char* name);

public:
	std::string					m_name;
	float						m_version;
	std::vector<CCBoneData>		m_bone_data;
	DATA_NAME_MAP				m_bone_data_map;
};



#endif //_CC_ARMATURE_DATA_H_
