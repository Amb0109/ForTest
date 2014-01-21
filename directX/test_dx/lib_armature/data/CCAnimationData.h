#ifndef _CC_ANIMATION_DATA_H_
#define _CC_ANIMATION_DATA_H_

#include "CCData.h"
#include "../cocos/CCMacro.h"
#include "../cocos/CCTweenFunction.h"

namespace CC {

enum CC_DLL CCBlendType
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

class CC_DLL CCFrameData : public CCBaseData
{
public:
	CCFrameData();
	virtual ~CCFrameData();

public:
	bool		load(Json::Value& json, CCDataCollection* data);
	void		copy(const CCFrameData* pFrameData);

public:
	int							m_iDisplayIndex;
	int							m_iFrameIndex;
	CCTweenType					m_eTweenEasing;
	bool						m_bTweenFrame;
	std::vector<float>			m_vecEasingParams;
	CCBlendType					m_eBlentType;
	std::string					m_sEvent;
};

class CC_DLL CCMovBoneData
{
public:
	bool			load(Json::Value& json, CCDataCollection* data);
	
	std::string&	getName() { return m_sName; }
	CCFrameData*	getFrameData(int index = 0);

public:
	std::string					m_sName;
	float						m_fDelay;
	std::vector<CCFrameData>	m_vecFrameData;

	int							m_iDuration;
	float						m_fMovementScale;
};

class CC_DLL CCMovData
{
public:
	bool			load(Json::Value& json, CCDataCollection* data);

	std::string&	getName() { return m_sName; }
	CCMovBoneData*	getMovementBoneData(int index = 0);
	CCMovBoneData*	getMovementBoneDataByName(const char* name);

public:
	std::string					m_sName;
	int							m_iDuration;
	bool						m_bLoop;
	int							m_iDurationTo;
	int							m_iDurationTween;
	int							m_iTweenEasing;
	float						m_fMovementScale;
	std::vector<CCMovBoneData>	m_vecMovementBoneDataList;
	DATA_NAME_MAP				m_mapMovementBoneDataMap;
};

class CC_DLL CCAnimationData
{
public:
	bool			load(Json::Value& json, CCDataCollection* data);

	std::string&	getName() { return m_sName; }
	CCMovData*		getMovementData(int index = 0);
	CCMovData*		getMovementDataByName(const char* name);
	int				getMovementCount();
	int				getMovementIndex(const char* name);
	
	CCDataCollection*			getDataCollection();

private:
	CCDataCollection*			m_pDataCollection;
	std::string					m_sName;
	std::vector<CCMovData>		m_vecMovementData;
	DATA_NAME_MAP				m_mapMovementDataMap;
};

} // namespace

#endif //_CC_ANIMATION_DATA_H_