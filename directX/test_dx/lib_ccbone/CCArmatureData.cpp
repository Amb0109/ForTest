#include "CCArmatureData.h"

bool CCArmatureData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_name = json.get(A_NAME, Json::Value("")).asString();
	m_version = json.get(VERSION, Json::Value("")).asFloat();

	if (data != NULL)
	{
		data->m_name = m_name;
		data->m_version = m_version;
	}

	Json::Value bone_data_json = json.get(BONE_DATA, Json::Value::jsonNull);
	if(bone_data_json.isArray())
	{
		for (unsigned i=0; i<bone_data_json.size(); ++i)
		{
			CCBoneData bone_data;
			if (!bone_data.load(bone_data_json[i], data)) return false;
			m_bone_data_map[bone_data.get_name()] = i;
			m_bone_data.push_back(bone_data);
		}
	}
	return true;
}

CCBoneData* CCArmatureData::get_bone_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_bone_data.size() <= (unsigned)index) return NULL;
	return &(m_bone_data[(unsigned)index]);
}

CCBoneData* CCArmatureData::get_bone_data_by_name( const char* name )
{
	if (m_bone_data_map.find(name) == m_bone_data_map.end()) return NULL;
	return get_bone_data(m_bone_data_map[name]);
}

bool CCBoneData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_name		= json.get(A_NAME, Json::Value("")).asString();
	m_parent	= json.get(A_PARENT, Json::Value("")).asString();

	if(!CCBaseData::load(json, data)) return false;
	// ??
	//m_arrow_x	= json.get("arrow_x", Json::Value(0.f)).asFloat();
	//m_arrow_x	= json.get("arrow_y", Json::Value(0.f)).asFloat();
	//m_effectbyskeleton = json.get("effectbyskeleton", Json::Value(false)).asBool();

	Json::Value display_data_json = json.get(DISPLAY_DATA, Json::Value::jsonNull);
	if(display_data_json.isArray())
	{
		for (unsigned i=0; i<display_data_json.size(); ++i)
		{
			CCDisplayData display_data;
			if (!display_data.load(display_data_json[i], data)) return false;
			m_display_data_map[display_data.get_name()] = i;
			m_display_data.push_back(display_data);
		}
	}
	return true;
}

CCDisplayData* CCBoneData::get_display_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_display_data.size() <= (unsigned)index) return NULL;
	return &(m_display_data[(unsigned)index]);
}

CCDisplayData* CCBoneData::get_display_data_by_name( const char* name )
{
	if (m_display_data_map.find(name) == m_display_data_map.end()) return NULL;
	return get_display_data(m_display_data_map[name]);
}

bool CCDisplayData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_displayType = json.get(A_DISPLAY_TYPE, Json::Value(0)).asInt();

	switch (m_displayType)
	{
	case CC_DISPLAY_SPRITE:
		{
			m_name = json.get(A_NAME, Json::Value("")).asString();

			Json::Value skin_data_json = json.get(SKIN_DATA, Json::Value::jsonNull);
			if (skin_data_json.isArray())
			{
				for (unsigned i=0; i<skin_data_json.size(); ++i)
				{
					CCBaseData skin_data;
					if (!skin_data.load(skin_data_json[i], data)) return false;
					m_skin_data.push_back(skin_data);
				}
			}
		}
		break;
	case CC_DISPLAY_ARMATURE:
		{
			m_name = json.get(A_NAME, Json::Value("")).asString();
		}
		break;
	case CC_DISPLAY_PARTICLE:
		{
			m_plist = json.get(A_PLIST, Json::Value("")).asString();
		}
		break;
	default:
		break;
	}
	return true;
}

CCBaseData* CCDisplayData::get_skin_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_skin_data.size() <= (unsigned)index) return NULL;
	return &(m_skin_data[(unsigned)index]);
}
