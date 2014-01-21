#include "CCAnimation.h"
#include "../armature/CCArmature.h"
#include "../armature/CCBone.h"
#include "../data/CCData.h"
#include "../data/CCArmatureData.h"
#include "../data/CCAnimationData.h"
#include "../animation/CCTween.h"

namespace CC {

CCAnimation::CCAnimation()
: m_pData(NULL)
, m_pArmature(NULL)
, m_vecTweenList()
, m_iCurrentMovementIndex(0)
, m_sCurrentMovementName()

, m_vecMovementList()
, m_bPlayMovementList(false)
, m_bMovementListLoop(false)
, m_iMovListDurationTo(-1)
, m_iMovListIndex(0)

{

}

CCAnimation::~CCAnimation()
{
	release();
}

void CCAnimation::release()
{
	m_pData = NULL;
	m_pArmature = NULL;
	m_vecTweenList.clear();
}

bool CCAnimation::init( CCAnimationData* data, CCArmature* armature )
{
	if (data == NULL) return false;
	if (armature == NULL) return false;

	m_pData = data;
	m_pArmature = armature;

	return true;
}

void CCAnimation::play( CCMovData* movData, int durationTo /*= -1*/, int durationTween /*= -1*/, int loop /*= -1*/, int tweenEasing /*= TWEEN_EASING_MAX*/ )
{
	if (m_pArmature == NULL) return;
	if (movData == NULL) return;

	m_pCurrentMovementData = movData;
	m_sCurrentMovementName = movData->getName();
	m_iCurrentMovementIndex = m_pData->getMovementIndex(movData->getName().c_str());

	m_bPlayMovementList = false;

	m_iRawDuration = movData->m_iDuration;
	m_fProcessScale = movData->m_fMovementScale;

	durationTo = (durationTo < 0) ? movData->m_iDurationTo : durationTo;
	
	durationTween = (durationTween < 0) ? movData->m_iDurationTween : durationTween;
	durationTween = (durationTween == 0) ? movData->m_iDuration : durationTween;

	loop = (loop < 0) ? movData->m_bLoop : loop;

	tweenEasing = (tweenEasing == TWEEN_EASING_MAX) ? movData->m_iTweenEasing : tweenEasing;

	CCProcessBase::play(durationTo, durationTween, loop, tweenEasing);

	if (m_iRawDuration == 0)
	{
		m_eLoopType = SINGLE_FRAME;
	}
	else
	{
		if (loop)
		{
			m_eLoopType = ANIMATION_TO_LOOP_FRONT;
		}
		else
		{
			m_eLoopType = ANIMATION_NO_LOOP;
		}
		m_iDurationTween = durationTween;
	}

	m_vecTweenList.clear();

	CCArmature::CC_BONE_MAP& bonesMap = 
		const_cast<CCArmature::CC_BONE_MAP&>(m_pArmature->getBonesMap());
	FOR_EACH (CCArmature::CC_BONE_MAP, bonesMap, it)
	{
		if (CCBone* pBone = dynamic_cast<CCBone*>(it->second))
		{
			CCTween* pTween = pBone->getTween();
			if (pTween == NULL) continue;

			CCBoneData* pBoneData = pBone->getData();
			if (pBoneData == NULL) continue;
			CCMovBoneData* pMovBoneData = movData->getMovementBoneDataByName(pBoneData->getName().c_str());

			if (pMovBoneData && pMovBoneData->getFrameData())
			{
				m_vecTweenList.push_back(pTween);
				pTween->play(pMovBoneData, durationTo, durationTween, loop, tweenEasing);
				pTween->setProcessScale(m_fProcessScale);
			}
			else
			{
				pTween->stop();
				pBone->setRenderDisplayIndex(-1);
			}
		}
	}

	update(0);
}

void CCAnimation::play( int movIndex, int durationTo /*= -1*/, int durationTween /*= -1*/, int loop /*= -1*/, int tweenEasing /*= TWEEN_EASING_MAX*/ )
{
	if (m_pData == NULL) return;
	play(m_pData->getMovementData(movIndex), durationTo, durationTween, loop, tweenEasing);
}

void CCAnimation::play( const char* movName, int durationTo /*= -1*/, int durationTween /*= -1*/, int loop /*= -1*/, int tweenEasing /*= TWEEN_EASING_MAX*/ )
{
	if (m_pData == NULL) return;
	play(m_pData->getMovementDataByName(movName), durationTo, durationTween, loop, tweenEasing);
}

void CCAnimation::playList( std::vector<int>& movIndexList, int durationTo, bool loop )
{
	if (m_pData == NULL) return;

	m_vecMovementList.clear();
	m_bMovementListLoop = loop;
	m_iMovListDurationTo = durationTo;
	m_bPlayMovementList = true;
	m_iMovListIndex = 0;

	FOR_EACH (std::vector<int>, movIndexList, it)
	{
		if (CCMovData* pMovData = m_pData->getMovementData(*it))
		{
			m_vecMovementList.push_back(pMovData);
		}
	}
	updateMovementList();
}

void CCAnimation::playList( std::vector<std::string>& movNameList, int durationTo, bool loop )
{
	if (m_pData == NULL) return;

	m_vecMovementList.clear();
	m_bMovementListLoop = loop;
	m_iMovListDurationTo = durationTo;
	m_bPlayMovementList = true;
	m_iMovListIndex = 0;

	FOR_EACH (std::vector<std::string>, movNameList, it)
	{
		if (CCMovData* pMovData = m_pData->getMovementDataByName(it->c_str()))
		{
			m_vecMovementList.push_back(pMovData);
		}
	}
	updateMovementList();
}

void CCAnimation::pause()
{
	FOR_EACH (std::vector<CCTween*>, m_vecTweenList, it)
	{
		if (CCTween* pTween = dynamic_cast<CCTween*>(*it))
		{
			pTween->pause();
		}
	}
	CCProcessBase::pause();
}

void CCAnimation::resume()
{
	FOR_EACH (std::vector<CCTween*>, m_vecTweenList, it)
	{
		if (CCTween* pTween = dynamic_cast<CCTween*>(*it))
		{
			pTween->resume();
		}
	}
	CCProcessBase::resume();
}

void CCAnimation::stop()
{
	FOR_EACH (std::vector<CCTween*>, m_vecTweenList, it)
	{
		if (CCTween* pTween = dynamic_cast<CCTween*>(*it))
		{
			pTween->stop();
		}
	}
	CCProcessBase::stop();
}

void CCAnimation::gotoAndPlay( int frameIndex )
{
	if (m_pCurrentMovementData == NULL) return;

	m_bIsPlaying = true;
	m_bIsComplete = m_bIsPause = false;

	CCProcessBase::gotoFrame(frameIndex);
	m_fCurrentPercent = (float)m_iCurFrameIndex / ((float)m_pCurrentMovementData->m_iDuration - 1);
	m_fCurrentFrame = m_iNextFrameIndex * m_fCurrentPercent;

	FOR_EACH (std::vector<CCTween*>, m_vecTweenList, it)
	{
		if (CCTween* tween = dynamic_cast<CCTween*>(*it))
		{
			tween->gotoAndPlay(frameIndex);
		}
	}

	update(0);
}

void CCAnimation::gotoAndPause( int frameIndex )
{
	gotoAndPlay(frameIndex);
	pause();
}

void CCAnimation::update( float delta )
{
	FOR_EACH(std::vector<CCTween*>, m_vecTweenList, it)
	{
		if (CCTween* tween = dynamic_cast<CCTween*>(*it))
		{
			tween->update(delta);
		}
	}
	CCProcessBase::update(delta);
}

void CCAnimation::updateMovementList()
{
	if (m_bPlayMovementList)
	{
		if (m_vecMovementList.size() == 0) return;
		m_bPlayMovementList = false;

		if (m_bMovementListLoop)
		{
			if (m_iMovListIndex >= (int)m_vecMovementList.size()) m_iMovListIndex = 0;
			play(m_vecMovementList[m_iMovListIndex], m_iMovListDurationTo, -1, 0);
			m_bPlayMovementList = true;
			m_iMovListIndex++;
		}
		else
		{
			if (m_iMovListIndex < (int)m_vecMovementList.size())
			{
				play(m_vecMovementList[m_iMovListIndex], m_iMovListDurationTo, -1, 0);
				m_bPlayMovementList = true;
				m_iMovListIndex++;
			}
			else
			{
				m_bPlayMovementList = false;
			}
		}
	}
}

void CCAnimation::updateHandler()
{
	if (m_fCurrentPercent >= 1)
	{
		switch(m_eLoopType)
		{
		case ANIMATION_NO_LOOP:
			{
				m_eLoopType = ANIMATION_MAX;
				m_fCurrentFrame = (m_fCurrentPercent - 1) * m_iNextFrameIndex;
				m_fCurrentPercent = m_fCurrentFrame / m_iDurationTween;

				if (m_fCurrentPercent >= 1.0f)
				{
				}
				else
				{
					m_iNextFrameIndex = m_iDurationTween;

					break;
				}
			}
			break;
		case ANIMATION_MAX:
		case SINGLE_FRAME:
			{
				m_fCurrentPercent = 1;
				m_bIsComplete = true;
				m_bIsPlaying = false;

				updateMovementList();
			}
			break;
		case ANIMATION_TO_LOOP_FRONT:
			{
				m_eLoopType = ANIMATION_LOOP_FRONT;
				m_fCurrentPercent = fmodf(m_fCurrentPercent, 1.f);
				m_fCurrentFrame = m_iNextFrameIndex == 0 ? 0 : fmodf(m_fCurrentFrame, (float)m_iNextFrameIndex);
				m_iNextFrameIndex = m_iDurationTween > 0 ? m_iDurationTween : 1;
			}
			break;
		default:
			{
				//m_fCurrentPercent = fmodf(m_fCurrentPercent, 1);
				m_fCurrentFrame = fmodf(m_fCurrentFrame, (float)m_iNextFrameIndex);
				//m_iToIndex = 0;
			}
			break;
		}
	}
}

}
