#include "CCBone.h"
#include "../armature/CCDisplay.h"
#include "../armature/CCArmature.h"
#include "../animation/CCTween.h"
#include "../data/CCData.h"
#include "../data/CCArmatureData.h"
#include "../data/CCAnimationData.h"
#include "../cocos/CCTransformHelp.h"
#include <algorithm>

namespace CC {

CCBone::CCBone()
: m_pData(NULL)
, m_pParent(NULL)
, m_pArmature(NULL)
, m_iDrawOrder(0)
, m_pRenderDisplay(NULL)
, m_pFrameData(NULL)
, m_pTween(NULL)
, m_vecChildBones()
, m_vecDisplayList()
, m_obWorldInfo()
, m_obWorldTransform(CCAffineTransformIdentity)
, m_fAlpha(1.f)
{
}

CCBone::~CCBone()
{
	release();
}

void CCBone::release()
{
	m_pData = NULL;
	m_pParent = NULL;
	m_pArmature = NULL;

	for (int i=0; i<(int)m_vecDisplayList.size(); ++i)
	{
		if (CCDisplay* display = m_vecDisplayList[i])
		{
			display->release();
			delete display;
		}
	}

	m_vecDisplayList.clear();
	m_vecChildBones.clear();
	m_iDrawOrder = 0;

	if (m_pFrameData) delete m_pFrameData;
	m_pFrameData = NULL;

	if (m_pTween) delete m_pTween;
	m_pTween = NULL;
}

bool CCBone::init( CCBoneData* bone_data, CCArmature* armature )
{
	if (bone_data == NULL) return false;
	if (armature == NULL) return false;

	m_pData = bone_data;
	m_iDrawOrder = bone_data->zOrder;
	m_pArmature = armature;

	for (int i=0; ; i++)
	{
		CCDisplayData* display_data = bone_data->getDisplayData(i);
		if (display_data == NULL) break;
		switch (display_data->m_eDisplayType)
		{
		case CC_DISPLAY_SPRITE:
			{
				CCDisplay* display = new CCDisplay();
				m_vecDisplayList.push_back(display);
				if (display != NULL)
				{
					display->init(display_data, this);
				}
			}
			break;
		default:
			m_vecDisplayList.push_back(NULL);
			break;
		}
	}

	return true;
}

bool CCBone::initAnimation( CCMovBoneData* movData )
{
	if (!m_pFrameData) m_pFrameData = new CCFrameData();
	if (!m_pFrameData) return false;
	if (!m_pTween) m_pTween = new CCTween();
	if (!m_pTween) return false;
	m_pTween->init(this);

	if (movData == NULL) return true;
	CCFrameData* pFrameData = movData->getFrameData();
	if (pFrameData == NULL) return true;
	m_pFrameData->copy(pFrameData);
	setRenderDisplayIndex(m_pFrameData->m_iDisplayIndex);
	return true;
}

bool CCBone::addChild( CCBone* childBone )
{
	if (childBone == NULL) return false;

	if(m_vecChildBones.end() == std::find(m_vecChildBones.begin(), m_vecChildBones.end(), childBone))
	{
		m_vecChildBones.push_back(childBone);
	}

	childBone->setParent(this);
	return true;
}

void CCBone::applyParentTransform( CCBone* parentBone )
{
	if (parentBone == NULL) return;
	float x = m_obWorldInfo.x;
	float y = m_obWorldInfo.y;
	m_obWorldInfo.x = x * parentBone->m_obWorldTransform.a + y * parentBone->m_obWorldTransform.c + parentBone->m_obWorldInfo.x;
	m_obWorldInfo.y = x * parentBone->m_obWorldTransform.b + y * parentBone->m_obWorldTransform.d + parentBone->m_obWorldInfo.y;
	m_obWorldInfo.scaleX = m_obWorldInfo.scaleX * parentBone->m_obWorldInfo.scaleX;
	m_obWorldInfo.scaleY = m_obWorldInfo.scaleY * parentBone->m_obWorldInfo.scaleY;
	m_obWorldInfo.skewX = m_obWorldInfo.skewX + parentBone->m_obWorldInfo.skewX;
	m_obWorldInfo.skewY = m_obWorldInfo.skewY + parentBone->m_obWorldInfo.skewY;
}

void CCBone::update( float delta )
{
	if (m_pParent)
		m_bTransformDirty |= m_pParent->isTransformDirty();

	if (m_bTransformDirty)
	{	
		if (m_pFrameData != NULL)
		{
			CCTransformHelp::nodeConcat(*m_pFrameData, *m_pData);
			m_pFrameData->scaleX -= 1;
			m_pFrameData->scaleY -= 1;

			m_obWorldInfo.x = m_pFrameData->x + m_obPosition.x;
			m_obWorldInfo.y = m_pFrameData->y + m_obPosition.y;
			m_obWorldInfo.scaleX = m_pFrameData->scaleX * m_fScaleX;
			m_obWorldInfo.scaleY = m_pFrameData->scaleY * m_fScaleY;
			m_obWorldInfo.skewX = m_pFrameData->skewX + m_fSkewX + m_fRotationX;
			m_obWorldInfo.skewY = m_pFrameData->skewY + m_fSkewY - m_fRotationY;

			setRenderDisplayIndex(m_pFrameData->m_iDisplayIndex);

			CCTransformHelp::nodeSub(*m_pFrameData, *m_pData);
			m_pFrameData->scaleX += 1;
			m_pFrameData->scaleY += 1;
		}

		if (m_pParent != NULL)
		{
			applyParentTransform(m_pParent);
		}

		CCTransformHelp::nodeToMatrix(m_obWorldInfo, m_obWorldTransform);
	}

	updateDisplay(delta);

	FOR_EACH (std::vector<CCBone*>, m_vecChildBones, it)
	{
		if (CCBone* pBone = dynamic_cast<CCBone*>(*it))
		{
			pBone->update(delta);
		}
	}
	m_bTransformDirty = false;
}

void CCBone::updateColor()
{
	if (m_pFrameData == NULL) return;
	m_fAlpha = m_pFrameData->a * 1.f / 255;
}

void CCBone::updateDisplay( float delta )
{
	if(NULL == m_pRenderDisplay) return;
	m_pRenderDisplay->update(delta);
}

void CCBone::setRenderDisplayIndex( int displayIndex/* = 0*/ )
{
	if (displayIndex < 0 || displayIndex >= (int)m_vecDisplayList.size())
		setRenderDisplay(NULL);
	else
		setRenderDisplay(m_vecDisplayList[displayIndex]);
}

void CCBone::setRenderDisplay( CCDisplay* display )
{
	m_pRenderDisplay = display;
}

} // namespace