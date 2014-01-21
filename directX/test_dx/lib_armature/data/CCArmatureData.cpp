#include "CCArmatureData.h"

#include "CCDataDef.h"

namespace CC {

bool CCArmatureData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_sName = json.get(A_NAME, Json::Value("")).asString();
	m_fVersion = json.get(VERSION, Json::Value("")).asFloat();

	if (data != NULL)
	{
		data->m_sName = m_sName;
		data->m_fVersion = m_fVersion;
		m_pDataCollection = data;
	}

	Json::Value boneDataJson = json.get(BONE_DATA, Json::Value::null);
	if(boneDataJson.isArray())
	{
		for (unsigned i=0; i<boneDataJson.size(); ++i)
		{
			CCBoneData boneData;
			if (!boneData.load(boneDataJson[i], data)) return false;
			m_mapBoneDataMap[boneData.getName()] = i;
			m_vecBoneDataList.push_back(boneData);
		}
	}
	return true;
}

CCBoneData* CCArmatureData::getBoneData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecBoneDataList.size() <= (unsigned)index) return NULL;
	return &(m_vecBoneDataList[(unsigned)index]);
}

CCBoneData* CCArmatureData::getBoneDataByName( const char* name )
{
	if (m_mapBoneDataMap.find(name) == m_mapBoneDataMap.end()) return NULL;
	return getBoneData(m_mapBoneDataMap[name]);
}

CCBoneData::CCBoneData()
: CCBaseData()
, m_sName()
, m_sParent()
, m_vecDisplayDataList()
, m_mapDisplayDataMap()
{

}

CCBoneData::~CCBoneData()
{

}

bool CCBoneData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_sName		= json.get(A_NAME, Json::Value("")).asString();
	m_sParent	= json.get(A_PARENT, Json::Value("")).asString();

	if(!CCBaseData::load(json, data)) return false;
	// ??
	//m_arrow_x	= json.get("arrow_x", Json::Value(0.f)).asFloat();
	//m_arrow_x	= json.get("arrow_y", Json::Value(0.f)).asFloat();
	//m_effectbyskeleton = json.get("effectbyskeleton", Json::Value(false)).asBool();

	Json::Value displayDataJson = json.get(DISPLAY_DATA, Json::Value::null);
	if(displayDataJson.isArray())
	{
		for (unsigned i=0; i<displayDataJson.size(); ++i)
		{
			CCDisplayData displayData;
			if (!displayData.load(displayDataJson[i], data)) return false;
			m_mapDisplayDataMap[displayData.getName()] = i;
			m_vecDisplayDataList.push_back(displayData);
		}
	}
	return true;
}

CCDisplayData* CCBoneData::getDisplayData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecDisplayDataList.size() <= (unsigned)index) return NULL;
	return &(m_vecDisplayDataList[(unsigned)index]);
}

CCDisplayData* CCBoneData::getDisplayDataByName( const char* name )
{
	if (m_mapDisplayDataMap.find(name) == m_mapDisplayDataMap.end()) return NULL;
	return getDisplayData(m_mapDisplayDataMap[name]);
}


bool CCDisplayData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_eDisplayType = json.get(A_DISPLAY_TYPE, Json::Value(0)).asInt();

	switch (m_eDisplayType)
	{
	case CC_DISPLAY_SPRITE:
		{
			m_sName = json.get(A_NAME, Json::Value("")).asString();

			Json::Value skinDataJson = json.get(SKIN_DATA, Json::Value::null);
			if (skinDataJson.isArray())
			{
				for (unsigned i=0; i<skinDataJson.size(); ++i)
				{
					CCBaseData skinData;
					if (!skinData.load(skinDataJson[i], data)) return false;
					m_vecSkinData.push_back(skinData);
				}
			}
		}
		break;
	case CC_DISPLAY_ARMATURE:
		{
			m_sName = json.get(A_NAME, Json::Value("")).asString();
		}
		break;
	case CC_DISPLAY_PARTICLE:
		{
			//m_sPlist = json.get(A_PLIST, Json::Value("")).asString();
		}
		break;
	default:
		break;
	}
	return true;
}

CCBaseData* CCDisplayData::getSkinData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecSkinData.size() <= (unsigned)index) return NULL;
	return &(m_vecSkinData[(unsigned)index]);
}

} // namespace