#ifndef _CC_ANIMATION_DATA_H_
#define _CC_ANIMATION_DATA_H_

#include "CCData.h"

enum CCTweenType
{
	TWEEN_EASING_MIN = -1,

	Linear,

	Sine_EaseIn,
	Sine_EaseOut,
	Sine_EaseInOut,


	Quad_EaseIn,
	Quad_EaseOut,
	Quad_EaseInOut,

	Cubic_EaseIn,
	Cubic_EaseOut,
	Cubic_EaseInOut,

	Quart_EaseIn,
	Quart_EaseOut,
	Quart_EaseInOut,

	Quint_EaseIn,
	Quint_EaseOut,
	Quint_EaseInOut,

	Expo_EaseIn,
	Expo_EaseOut,
	Expo_EaseInOut,

	Circ_EaseIn,
	Circ_EaseOut,
	Circ_EaseInOut,

	Elastic_EaseIn,
	Elastic_EaseOut,
	Elastic_EaseInOut,

	Back_EaseIn,
	Back_EaseOut,
	Back_EaseInOut,

	Bounce_EaseIn,
	Bounce_EaseOut,
	Bounce_EaseInOut,

	TWEEN_EASING_MAX = 10000
};

enum CCBlendType
{
	BLEND_NORMAL = 0,
	BLEND_LAYER,
	BLEND_DARKEN,
	BLEND_MULTIPLY,
	BLEND_LIGHTEN,
	BLEND_SCREEN,
	BLEND_OVERLAY,
	BLEND_HARD_LIGHT,
	BLEND_ADD,
	BLEND_SUBSTRACT,
	BLEND_DIFFERENCE,
	BLEND_INVERT,
	BLEND_ALPHA,
	BLEND_ERASE
};

class CCFrameData : public CCBaseData
{
public:
	bool		load(Json::Value& json, CCData* data);

public:
	int							m_display_index;
	int							m_frame_index;
	CCTweenType					m_tween_easing;
	bool						m_tween_frame;
	CCBlendType					m_blent_type;
	std::string					m_event;
};

class CCMovBoneData
{
public:
	bool			load(Json::Value& json, CCData* data);
	
	std::string&	get_name() { return m_name; }
	CCFrameData*	get_frame_data(int index = 0);

public:
	std::string					m_name;
	float						m_delay;
	std::vector<CCFrameData>	m_frame_data;
};

class CCMovData
{
public:
	bool			load(Json::Value& json, CCData* data);

	std::string&	get_name() { return m_name; }
	CCMovBoneData*	get_mov_bone_data(int index = 0);
	CCMovBoneData*	get_mov_bone_data_by_name(const char* name);

public:
	std::string					m_name;
	int							m_duration;
	bool						m_loop;
	int							m_duration_to;
	int							m_duration_tween;
	int							m_tween_easing;
	float						m_movement_scale;
	std::vector<CCMovBoneData>	m_mov_bone_data;
	DATA_NAME_MAP				m_mov_bone_data_map;
};

class CCAnimationData
{
public:
	bool			load(Json::Value& json, CCData* data);

	std::string&	get_name() { return m_name; }
	CCMovData*		get_mov_data(int index = 0);
	CCMovData*		get_mov_data_by_name(const char* name);
	
private:
	std::string					m_name;
	std::vector<CCMovData>		m_mov_data;
	DATA_NAME_MAP				m_mov_data_map;
};


#endif //_CC_ANIMATION_DATA_H_