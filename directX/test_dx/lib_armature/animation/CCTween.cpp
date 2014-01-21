#include "CCTween.h"
#include "../armature/CCBone.h"
#include "../armature/CCArmature.h"
#include "../animation/CCAnimation.h"
#include "../cocos/CCTweenFunction.h"
#include "../cocos/CCTransformHelp.h"
#include "../data/CCArmatureData.h"
#include "../data/CCAnimationData.h"

namespace CC {


CCTween::CCTween()
: m_pBone(NULL)
, m_pFrameData(NULL)
, m_pMovementBoneData(NULL)
{

}

CCTween::~CCTween( void )
{
	release();
}

bool CCTween::init( CCBone* bone )
{
	if (bone == NULL) return false;

	m_pBone = bone;
	m_pFrameData = m_pBone->getFrameData();
	return true;
}

void CCTween::release()
{
	m_pBone = NULL;
	m_pFrameData = NULL;
}

void CCTween::play( CCMovBoneData* movBoneData, int durationTo /*= -1*/, int durationTween /*= -1*/, int loop /*= -1*/, int tweenEasing /*= TWEEN_EASING_MAX*/ )
{
	if (movBoneData == NULL) return;
	if (m_pFrameData == NULL) return;

	CCProcessBase::play(durationTo, durationTween, loop, tweenEasing);

	bool bMovBoneChanged = (m_pMovementBoneData != movBoneData);
	
	m_pMovementBoneData = movBoneData;
	m_iRawDuration = m_pMovementBoneData->m_iDuration;

	CCFrameData* pNextKeyFrame = movBoneData->getFrameData();
	if (pNextKeyFrame == NULL) return;
	m_pFrameData->m_iDisplayIndex = pNextKeyFrame->m_iDisplayIndex;

	if (loop) m_eLoopType = ANIMATION_TO_LOOP_FRONT;
	else m_eLoopType = ANIMATION_NO_LOOP;

	if (m_iRawDuration == 0 || m_pMovementBoneData->m_vecFrameData.size() == 1)
	{
		m_eLoopType = SINGLE_FRAME;
		if(durationTo == 0)
		{
			setBetween(pNextKeyFrame, pNextKeyFrame);
		}
		else
		{
			setBetween(m_pFrameData, pNextKeyFrame);
		}
		m_eFrameTweenEasing = Linear;
	}
	else if (m_pMovementBoneData->m_vecFrameData.size() > 1)
	{
		m_iDurationTween = durationTween/* * m_mov_bone_data->m_movement_scale*/;

		if (loop && m_pMovementBoneData->m_fDelay != 0)
		{
			setBetween(m_pFrameData, tweenNodeTo(updateFrameData(1 - m_pMovementBoneData->m_fDelay), &m_obBetween));
		}
		else
		{
			if (!bMovBoneChanged || durationTo == 0)
			{
				setBetween(pNextKeyFrame, pNextKeyFrame);
			}
			else
			{
				setBetween(m_pFrameData, pNextKeyFrame);
			}
		}
	}
}

void CCTween::stop()
{
	if (m_pFrameData == NULL) return;
	m_pFrameData->m_iDisplayIndex = -1;
}

void CCTween::gotoAndPlay( int frameIndex )
{
	CCProcessBase::gotoFrame(frameIndex);

	m_fTotalDuration = 0;
	m_fBetweenDuration = 0;
	m_iFromIndex = m_iToIndex = 0;

	m_bIsPlaying = true;
	m_bIsComplete = m_bIsPause = false;

	m_fCurrentPercent = (float)m_iCurFrameIndex / ((float)m_iRawDuration - 1);
	m_fCurrentFrame = m_iNextFrameIndex * m_fCurrentPercent;
}

void CCTween::updateHandler()
{
	if (m_fCurrentPercent >= 1)
	{
		switch(m_eLoopType)
		{
		case SINGLE_FRAME:
			{
				m_fCurrentPercent = 1;
				m_bIsComplete = true;
				m_bIsPlaying = false;
			}
			break;
		case ANIMATION_NO_LOOP:
			{
				m_eLoopType = ANIMATION_MAX;


				if (m_iDurationTween <= 0)
				{
					m_fCurrentPercent = 1;
				}
				else
				{
					m_fCurrentPercent = (m_fCurrentPercent - 1) * m_iNextFrameIndex / m_iDurationTween;
				}

				if (m_fCurrentPercent >= 1)
				{
					m_fCurrentPercent = 1;
					m_bIsComplete = true;
					m_bIsPlaying = false;
					break;
				}
				else
				{
					m_iNextFrameIndex = m_iDurationTween;
					m_fCurrentFrame = m_fCurrentPercent * m_iNextFrameIndex;

					m_fTotalDuration = 0;
					m_fBetweenDuration = 0;
					m_iFromIndex = m_iToIndex = 0;
					break;
				}
			}
			break;
		case ANIMATION_TO_LOOP_FRONT:
			{
				m_eLoopType = ANIMATION_LOOP_FRONT;

				m_iNextFrameIndex = m_iDurationTween > 0 ? m_iDurationTween : 1;

				if (m_pMovementBoneData->m_fDelay != 0)
				{
					//
					m_fCurrentFrame = (1 - m_pMovementBoneData->m_fDelay) * (float)m_iNextFrameIndex;
					m_fCurrentPercent = m_fCurrentFrame / m_iNextFrameIndex;
				}
				else
				{
					m_fCurrentPercent = 0;
					m_fCurrentFrame = 0;
				}

				m_fTotalDuration = 0;
				m_fBetweenDuration = 0;
				m_iFromIndex = m_iToIndex = 0;
			}
			break;
		case ANIMATION_MAX:
			{
				m_fCurrentPercent = 1;
				m_bIsComplete = true;
				m_bIsPlaying = false;
			}
			break;
		default:
			{
				m_fCurrentFrame = fmodf(m_fCurrentFrame, m_iNextFrameIndex);

				m_fTotalDuration = 0;
				m_fBetweenDuration = 0;
				m_iFromIndex = m_iToIndex = 0;
			}
			break;
		}
	}

	if (m_fCurrentPercent < 1 && m_eLoopType <= ANIMATION_TO_LOOP_BACK)
	{
		m_fCurrentPercent = sin(m_fCurrentPercent * M_PI_2);
	}

	float percent  = m_fCurrentPercent;

	if (m_eLoopType > ANIMATION_TO_LOOP_BACK)
	{
		percent = updateFrameData(percent);
	}

	if(m_eFrameTweenEasing != TWEEN_EASING_MAX)
	{
		tweenNodeTo(percent);
	}
}

float CCTween::updateFrameData(float currentPercent)
{
	if (m_pMovementBoneData == NULL) return currentPercent;

    if (currentPercent > 1 && m_pMovementBoneData->m_fDelay != 0)
    {
        currentPercent = fmodf(currentPercent, 1);
    }

    float playedTime = (float)(m_iRawDuration - 1) * currentPercent;


    //! If play to current frame's front or back, then find current frame again
    if (playedTime < m_fTotalDuration || playedTime >= m_fTotalDuration + m_fBetweenDuration)
    {
        /*
         *  Get frame length, if m_iToIndex >= _length, then set m_iToIndex to 0, start anew.
         *  m_iToIndex is next index will play
         */
        int length = (int)m_pMovementBoneData->m_vecFrameData.size();
		std::vector<CCFrameData>& frames = m_pMovementBoneData->m_vecFrameData;

        CCFrameData *pFromFrame = NULL;
        CCFrameData *pToFrame = NULL;

        if (playedTime < frames[0].m_iFrameIndex)
        {
            pFromFrame = pToFrame = &frames[0];
            setBetween(pFromFrame, pToFrame);
            return m_fCurrentPercent;
        }
        
        if(playedTime >= frames[length - 1].m_iFrameIndex)
        {
            if (m_bPassLastFrame)
            {
                pFromFrame = pToFrame = &frames[length - 1];
                setBetween(pFromFrame, pToFrame);
                return m_fCurrentPercent;
            }
            m_bPassLastFrame = true;
        }
        else
        {
            m_bPassLastFrame = false;
        }


        do
        {
            m_iFromIndex = m_iToIndex;
            pFromFrame = &frames[m_iFromIndex];
            m_fTotalDuration  = pFromFrame->m_iFrameIndex;

            m_iToIndex = m_iFromIndex + 1;
            if (m_iToIndex >= length)
            {
                m_iToIndex = 0;
            }

            pToFrame = &frames[m_iToIndex];

            ////! Guaranteed to trigger frame event
            //if(from->m_event.length() != 0 && !m_animation->isIgnoreFrameEvent())
            //{
            //    m_animation->frameEvent(m_bone, from->m_event.c_str(), from->m_frame_index, playedTime);
            //}

            if (playedTime == pFromFrame->m_iFrameIndex || (m_bPassLastFrame && m_iFromIndex == length-1))
            {
                break;
            }
        }
        while (playedTime < pFromFrame->m_iFrameIndex || playedTime >= pToFrame->m_iFrameIndex);

        m_fBetweenDuration = pToFrame->m_iFrameIndex - pFromFrame->m_iFrameIndex;

        m_eFrameTweenEasing = pFromFrame->m_eTweenEasing;

        setBetween(pFromFrame, pToFrame, false);

    }
    currentPercent = m_fBetweenDuration == 0 ? 0 : (playedTime - m_fTotalDuration) / (float)m_fBetweenDuration;


    /*
     *  If frame tween easing equal to TWEEN_EASING_MAX, then it will not do tween.
     */

    CCTweenType tweenType = (m_eFrameTweenEasing != Linear) ? m_eFrameTweenEasing : m_eTweenEasing;
    if (tweenType != TWEEN_EASING_MAX && tweenType != Linear && !m_bPassLastFrame)
    {
        currentPercent = CCTweenFunction::tweenTo(currentPercent, tweenType, m_obFrom.m_vecEasingParams);
    }

    return currentPercent;
}

void CCTween::setBetween( CCFrameData* from, CCFrameData* to, bool limit /*= true*/ )
{
	do
	{
		if(from->m_iDisplayIndex < 0 && to->m_iDisplayIndex >= 0)
		{
			m_obFrom.copy(to);
			m_obBetween.subtract(to, to, limit);
			break;
		}
		else if(to->m_iDisplayIndex < 0 && from->m_iDisplayIndex >= 0)
		{
			m_obFrom.copy(from);
			m_obBetween.subtract(to, to, limit);
			break;
		}

		m_obFrom.copy(from);
		m_obBetween.subtract(from, to, limit);
	}
	while (0);

	if (!from->m_bTweenFrame)
	{
		m_pFrameData->copy(from);
		m_pFrameData->m_bTweenFrame = true;
	}

	if (m_pFrameData->m_iDisplayIndex != from->m_iDisplayIndex)
	{
		m_pFrameData->m_iDisplayIndex = from->m_iDisplayIndex;
		m_pBone->setTransformDirty(true);
	}

	if (m_pFrameData->zOrder != from->zOrder)
	{
		m_pFrameData->zOrder = from->zOrder;
		m_pBone->setTransformDirty(true);
	}
}

CCFrameData *CCTween::tweenNodeTo(float percent, CCFrameData *node)
{
	node = node == NULL ? m_pFrameData : node;
	if (!m_obFrom.m_bTweenFrame)
	{
		percent = 0;
	}

	node->x = m_obFrom.x + percent * m_obBetween.x;
	node->y = m_obFrom.y + percent * m_obBetween.y;
	node->scaleX = m_obFrom.scaleX + percent * m_obBetween.scaleX;
	node->scaleY = m_obFrom.scaleY + percent * m_obBetween.scaleY;
	node->skewX = m_obFrom.skewX + percent * m_obBetween.skewX;
	node->skewY = m_obFrom.skewY + percent * m_obBetween.skewY;

	m_pBone->setTransformDirty(true);

	if (node && m_obBetween.isUseColorInfo)
	{
		tweenColorTo(percent, node);
	}

	return node;
}

void CCTween::tweenColorTo(float percent, CCFrameData *node)
{
	node->a = m_obFrom.a + percent * m_obBetween.a;
	node->r = m_obFrom.r + percent * m_obBetween.r;
	node->g = m_obFrom.g + percent * m_obBetween.g;
	node->b = m_obFrom.b + percent * m_obBetween.b;
	m_pBone->updateColor();
}

}
