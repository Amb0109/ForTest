#include "CCAnimationData.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

bool CCAnimationData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_name = json.get(A_NAME, Json::Value("")).asString();

	Json::Value mov_data_json = json.get(MOVEMENT_DATA, Json::Value::jsonNull);
	if (mov_data_json.isArray())
	{
		for (unsigned i=0; i<mov_data_json.size(); ++i)
		{
			CCMovData mov_data;
			if (!mov_data.load(mov_data_json[i], data)) return false;
			m_mov_data_map[mov_data.get_name()] = i;
			m_mov_data.push_back(mov_data);
		}
	}
	return true;
}

CCMovData* CCAnimationData::get_mov_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_mov_data.size() <= (unsigned)index) return NULL;
	return &(m_mov_data[(unsigned)index]);
}

CCMovData* CCAnimationData::get_mov_data_by_name( const char* name )
{
	if (m_mov_data_map.find(name) == m_mov_data_map.end()) return NULL;
	return get_mov_data(m_mov_data_map[name]);
}

bool CCMovData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_name = json.get(A_NAME, Json::Value("")).asString();
	m_duration = json.get(A_DURATION, Json::Value(0)).asInt();
	m_loop = json.get(A_LOOP, Json::Value(true)).asBool();
	m_duration_to = json.get(A_DURATION_TO, Json::Value(0)).asInt();
	m_duration_tween = json.get(A_DURATION_TWEEN, Json::Value(0)).asInt();
	m_tween_easing = json.get(A_TWEEN_EASING, Json::Value(0)).asInt();
	m_movement_scale = json.get(A_MOVEMENT_SCALE, Json::Value(1.f)).asFloat();

	Json::Value mov_bone_data_json = json.get(MOVEMENT_BONE_DATA, Json::Value::jsonNull);
	if (mov_bone_data_json.isArray())
	{
		for (unsigned i=0; i<mov_bone_data_json.size(); ++i)
		{
			CCMovBoneData mov_bone_data;
			if (!mov_bone_data.load(mov_bone_data_json[i], data)) return false;
			m_mov_bone_data_map[mov_bone_data.get_name()] = i;
			m_mov_bone_data.push_back(mov_bone_data);
		}
	}
	return true;
}

CCMovBoneData* CCMovData::get_mov_bone_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_mov_bone_data.size() <= (unsigned)index) return NULL;
	return &(m_mov_bone_data[(unsigned)index]);
}

CCMovBoneData* CCMovData::get_mov_bone_data_by_name( const char* name )
{
	if (m_mov_bone_data_map.find(name) == m_mov_bone_data_map.end()) return NULL;
	return get_mov_bone_data(m_mov_bone_data_map[name]);
}

bool CCMovBoneData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_name = json.get(A_NAME, Json::Value("")).asString();
	m_delay = json.get(A_MOVEMENT_DELAY, Json::Value(0.f)).asFloat();

	Json::Value frame_data_json = json.get(FRAME_DATA, Json::Value::jsonNull);
	if (frame_data_json.isArray())
	{
		for (unsigned i=0; i<frame_data_json.size(); ++i)
		{
			CCFrameData frame_data;
			if (!frame_data.load(frame_data_json[i], data)) return false;
			m_frame_data.push_back(frame_data);
		}
	}

	if (data->m_version < VERSION_CHANGE_ROTATION_RANGE)
	{
		//! Change rotation range from (-180 -- 180) to (-infinity -- infinity)
		for (int i=(int)m_frame_data.size()-1; i>0; --i)
		{
			float difSkewX = m_frame_data[i].skewX -  m_frame_data[i - 1].skewX;
			float difSkewY = m_frame_data[i].skewY -  m_frame_data[i - 1].skewY;
			if (difSkewX < -M_PI || difSkewX > M_PI)
			{
				m_frame_data[i - 1].skewX = difSkewX < 0 ?
					m_frame_data[i - 1].skewX - 2 * M_PI : m_frame_data[i - 1].skewX + 2 * M_PI;
			}

			if (difSkewY < -M_PI || difSkewY > M_PI)
			{
				m_frame_data[i - 1].skewY = difSkewY < 0 ?
					m_frame_data[i - 1].skewY - 2 * M_PI : m_frame_data[i - 1].skewY + 2 * M_PI;
			}
		}
	}
	return true;
}

CCFrameData* CCMovBoneData::get_frame_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_frame_data.size() <= (unsigned)index) return NULL;
	return &(m_frame_data[(unsigned)index]);
}

bool CCFrameData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_display_index = json.get(A_DISPLAY_INDEX, Json::Value(0)).asInt();
	m_frame_index = json.get(A_FRAME_INDEX, Json::Value(0)).asInt();
	m_tween_easing = (CCTweenType)json.get(A_TWEEN_EASING, Json::Value(0)).asInt();
	m_tween_frame = json.get(A_TWEEN_FRAME, Json::Value(false)).asBool();
	m_blent_type = (CCBlendType)json.get(A_BLEND_TYPE, Json::Value(0)).asInt();
	m_event = json.get(A_EVENT, Json::Value("")).asString();

	return CCBaseData::load(json, data);
}
