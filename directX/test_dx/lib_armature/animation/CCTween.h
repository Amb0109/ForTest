#ifndef __CCTWEEN_H__
#define __CCTWEEN_H__

#include "CCProcessBase.h"
#include "../cocos/CCTweenFunction.h"
#include "../data/CCAnimationData.h"

namespace CC {

class CCBone;
class CCArmatureAnimation;
class CCMovBoneData;
class CCFrameData;

class  CCTween : public CCProcessBase
{
public:
    CCTween();
    virtual ~CCTween();

public:
	bool init(CCBone* bone);
	void release();

	void play(CCMovBoneData* movBoneData, int durationTo = -1, int durationTween = -1, int loop = -1, int tweenEasing = TWEEN_EASING_MAX);
	//void pause();
	//void resume();
	virtual void stop();

	void gotoAndPlay(int frameIndex);

	virtual void updateHandler();
	float updateFrameData(float currentPercent);
	void setBetween(CCFrameData* from, CCFrameData* to, bool limit = true);
	CCFrameData* tweenNodeTo(float percent, CCFrameData *node = NULL);
	void tweenColorTo(float percent, CCFrameData *node);

private:
	MEMBER_GET_PTR(CCBone*, m_pBone, Bone);
	MEMBER_GET_PTR(CCFrameData*, m_pFrameData, FrameData);
	MEMBER_GET_PTR(CCMovBoneData*, m_pMovementBoneData, MovementBoneData);

	float					m_fTotalDuration;
	float					m_fBetweenDuration;
	bool					m_bPassLastFrame;
	CCTweenType				m_eFrameTweenEasing;
	int						m_iFromIndex;
	int						m_iToIndex;

	CCFrameData				m_obFrom;
	CCFrameData				m_obTo;
	CCFrameData				m_obBetween;
};

}

#endif /*__CCTWEEN_H__*/
