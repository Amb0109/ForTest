#include "CCAnimationData.h"
#include <math.h>

#include "CCDataDef.h"

namespace CC {

bool CCAnimationData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_pDataCollection = data;
	m_sName = json.get(A_NAME, Json::Value("")).asString();

	Json::Value movDataJson = json.get(MOVEMENT_DATA, Json::Value::null);
	if (movDataJson.isArray())
	{
		for (unsigned i=0; i<movDataJson.size(); ++i)
		{
			CCMovData movData;
			if (!movData.load(movDataJson[i], data)) return false;
			m_mapMovementDataMap[movData.getName()] = i;
			m_vecMovementData.push_back(movData);
		}
	}
	return true;
}

CCMovData* CCAnimationData::getMovementData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecMovementData.size() <= (unsigned)index) return NULL;
	return &(m_vecMovementData[(unsigned)index]);
}

CCMovData* CCAnimationData::getMovementDataByName( const char* name )
{
	if (m_mapMovementDataMap.find(name) == m_mapMovementDataMap.end()) return NULL;
	return getMovementData(m_mapMovementDataMap[name]);
}

CCDataCollection* CCAnimationData::getDataCollection()
{
	return m_pDataCollection;
}

int CCAnimationData::getMovementCount()
{
	return (int)m_vecMovementData.size();
}

int CCAnimationData::getMovementIndex( const char* name )
{
	if (m_mapMovementDataMap.find(name) == m_mapMovementDataMap.end()) return 0;
	return m_mapMovementDataMap[name];
}

bool CCMovData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_sName = json.get(A_NAME, Json::Value("")).asString();
	m_iDuration = json.get(A_DURATION, Json::Value(0)).asInt();
	m_bLoop = json.get(A_LOOP, Json::Value(true)).asBool();
	m_iDurationTo = json.get(A_DURATION_TO, Json::Value(0)).asInt();
	m_iDurationTween = json.get(A_DURATION_TWEEN, Json::Value(0)).asInt();
	m_iTweenEasing = json.get(A_TWEEN_EASING, Json::Value(0)).asInt();
	
	if (!json.isMember(A_DURATION))
	{
		m_fMovementScale = 1.f;
	}
	else
	{
		m_fMovementScale = json.get(A_MOVEMENT_SCALE, Json::Value(1.f)).asFloat();
	}
	

	Json::Value movBoneDataJson = json.get(MOVEMENT_BONE_DATA, Json::Value::null);
	if (movBoneDataJson.isArray())
	{
		for (unsigned i=0; i<movBoneDataJson.size(); ++i)
		{
			CCMovBoneData movBoneData;
			if (!movBoneData.load(movBoneDataJson[i], data)) return false;
			m_mapMovementBoneDataMap[movBoneData.getName()] = i;
			movBoneData.m_iDuration = m_iDuration;
			//mov_bone_data.m_movement_scale = m_movement_scale;
			m_vecMovementBoneDataList.push_back(movBoneData);
		}
	}
	return true;
}

CCMovBoneData* CCMovData::getMovementBoneData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecMovementBoneDataList.size() <= (unsigned)index) return NULL;
	return &(m_vecMovementBoneDataList[(unsigned)index]);
}

CCMovBoneData* CCMovData::getMovementBoneDataByName( const char* name )
{
	if (m_mapMovementBoneDataMap.find(name) == m_mapMovementBoneDataMap.end()) return NULL;
	return getMovementBoneData(m_mapMovementBoneDataMap[name]);
}

bool CCMovBoneData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_sName = json.get(A_NAME, Json::Value("")).asString();
	m_fDelay = json.get(A_MOVEMENT_DELAY, Json::Value(0.f)).asFloat();

	m_fMovementScale = 1.f;

	Json::Value frameDataJson = json.get(FRAME_DATA, Json::Value::null);
	if (frameDataJson.isArray())
	{
		for (unsigned i=0; i<frameDataJson.size(); ++i)
		{
			CCFrameData frameData;
			if (!frameData.load(frameDataJson[i], data)) return false;
			m_vecFrameData.push_back(frameData);
		}
	}

	if (data->m_fVersion < VERSION_CHANGE_ROTATION_RANGE)
	{
		//! Change rotation range from (-180 -- 180) to (-infinity -- infinity)
		for (int i=(int)m_vecFrameData.size()-1; i>0; --i)
		{
			float difSkewX = m_vecFrameData[i].skewX -  m_vecFrameData[i - 1].skewX;
			float difSkewY = m_vecFrameData[i].skewY -  m_vecFrameData[i - 1].skewY;
			if (difSkewX < -M_PI || difSkewX > M_PI)
			{
				m_vecFrameData[i - 1].skewX = difSkewX < 0 ?
					m_vecFrameData[i - 1].skewX - 2.f * M_PI : m_vecFrameData[i - 1].skewX + 2.f * M_PI;
			}

			if (difSkewY < -M_PI || difSkewY > M_PI)
			{
				m_vecFrameData[i - 1].skewY = difSkewY < 0 ?
					m_vecFrameData[i - 1].skewY - 2.f * M_PI : m_vecFrameData[i - 1].skewY + 2.f * M_PI;
			}
		}
	}
	return true;
}

CCFrameData* CCMovBoneData::getFrameData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecFrameData.size() <= (unsigned)index) return NULL;
	return &(m_vecFrameData[(unsigned)index]);
}

CCFrameData::CCFrameData()
: m_iDisplayIndex(0)
, m_iFrameIndex(0)
, m_eTweenEasing(Linear)
, m_bTweenFrame(0)
, m_vecEasingParams()
, m_eBlentType(BLEND_NORMAL)
, m_sEvent()
{

}

CCFrameData::~CCFrameData()
{

}

bool CCFrameData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_iDisplayIndex = json.get(A_DISPLAY_INDEX, Json::Value(0)).asInt();
	m_iFrameIndex = json.get(A_FRAME_INDEX, Json::Value(0)).asInt();
	m_eTweenEasing = (CCTweenType)json.get(A_TWEEN_EASING, Json::Value(0)).asInt();
	m_bTweenFrame = json.get(A_TWEEN_FRAME, Json::Value(false)).asBool();
	
	m_vecEasingParams.clear();
	Json::Value easingParamsJson = json.get(A_EASING_PARAM, Json::Value::null);
	if ((!easingParamsJson.isNull()) && easingParamsJson.isArray())
	{
		for (unsigned i=0; i < easingParamsJson.size(); ++i)
		{
			m_vecEasingParams.push_back(easingParamsJson[i].asFloat());
		}
	}

	m_eBlentType = (CCBlendType)json.get(A_BLEND_TYPE, Json::Value(0)).asInt();
	m_sEvent = json.get(A_EVENT, Json::Value("")).asString();

	return CCBaseData::load(json, data);
}

void CCFrameData::copy( const CCFrameData* pFrameData )
{
	CCBaseData::copy(pFrameData);
	m_iDisplayIndex		= pFrameData->m_iDisplayIndex;
	m_iFrameIndex		= pFrameData->m_iFrameIndex;
	m_eTweenEasing		= pFrameData->m_eTweenEasing;
	m_bTweenFrame		= pFrameData->m_bTweenFrame;
	m_eBlentType		= pFrameData->m_eBlentType;
	m_sEvent			= pFrameData->m_sEvent;
	m_vecEasingParams	= pFrameData->m_vecEasingParams;
}

} // namespace