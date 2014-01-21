#ifndef __CCANIMATION_H__
#define __CCANIMATION_H__

#include "CCProcessBase.h"
#include <queue>

namespace CC {

class CCArmature;
class CCBone;
class CCAnimationData;
class CCMovData;
class CCTween;

enum MovementEventType
{
    START,
    COMPLETE,
    LOOP_COMPLETE
};

struct CCFrameEvent
{
    CCBone *bone;
    const char *frameEventName;
    int originFrameIndex;
    int currentFrameIndex;
};

struct CCMovementEvent
{
    CCArmature *armature;
    MovementEventType movementType;
    const char *movementID;
};

/**
 *  @lua NA
 */
class  CCAnimation : public CCProcessBase
{
public:
	CCAnimation();
	virtual ~CCAnimation();

public:
	bool init(CCAnimationData* data, CCArmature* armature);
	void release();

	void play(CCMovData* movData, int durationTo = -1, int durationTween = -1, int loop = -1, int tweenEasing = TWEEN_EASING_MAX);
	void play(int movIndex, int durationTo = -1, int durationTween = -1, int loop = -1, int tweenEasing = TWEEN_EASING_MAX);
	void play(const char* movName, int durationTo = -1, int durationTween = -1, int loop = -1, int tweenEasing = TWEEN_EASING_MAX);
	void playList(std::vector<int>& movIndexList, int durationTo, bool loop);
	void playList(std::vector<std::string>& movNameList, int durationTo, bool loop);
	virtual void pause();
	virtual void resume();
	virtual void stop();

	void gotoAndPlay(int frameIndex);
	void gotoAndPause(int frameIndex);

	void update(float delta);
	void updateMovementList();

	virtual void updateHandler();

private:
	MEMBER_GET_PTR(CCAnimationData*, m_pData, Data);
	MEMBER_GET_PTR(CCArmature*, m_pArmature, Armature);

	MEMBER_GET_PTR(CCMovData*, m_pCurrentMovementData, CurrentMovementData);
	MEMBER_GET_CREF(int, m_iCurrentMovementIndex, CurrentMovementIndex);
	MEMBER_GET_CREF(std::string, m_sCurrentMovementName, CurrentMovementName);

	std::vector<CCMovData*>		m_vecMovementList;
	bool						m_bPlayMovementList;
	bool						m_bMovementListLoop;
	int							m_iMovListDurationTo;
	int							m_iMovListIndex;

	std::vector<CCTween*>		m_vecTweenList;
};

}

#endif /*__CCANIMATION_H__*/
