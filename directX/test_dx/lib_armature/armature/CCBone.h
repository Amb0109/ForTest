#ifndef _CC_BONE_H_
#define _CC_BONE_H_

#include "../cocos/CCMacro.h"
#include "../cocos/CCAffineTransform.h"
#include "../cocos/CCNode.h"
#include "../data/CCData.h"
#include <vector>

namespace CC {

class CCBaseData;
class CCBoneData;
class CCFrameData;
class CCMovBoneData;
class CCArmature;
class CCDisplay;
class CCTween;

class CC_DLL CCBone : public CCNode
{
public:
	CCBone();
	virtual ~CCBone();

public:
	bool init(CCBoneData* data, CCArmature* armature);
	bool initAnimation(CCMovBoneData* movData);
	void release();

	bool addChild(CCBone* childBone);

	void setRenderDisplayIndex(int displayIndex = 0);
	void setRenderDisplay(CCDisplay* display);

	void update(float delta);
	void applyParentTransform(CCBone* parentBone);
	void updateDisplay(float delta);
	void updateColor();

private:
	MEMBER_GET_PTR(CCBoneData*, m_pData, Data);
	MEMBER_GET_PTR(CCTween*, m_pTween, Tween);
	MEMBER_GET_PTR(CCFrameData*, m_pFrameData, FrameData);
	MEMBER_GET_SET_PTR(CCBone*, m_pParent, Parent);
	MEMBER_GET_SET_PTR(CCArmature*, m_pArmature, Armature);

	std::vector<CCBone*>	m_vecChildBones;
	MEMBER_GET_CREF(int, m_iDrawOrder, DrawOrder);

	std::vector<CCDisplay*>	m_vecDisplayList;
	MEMBER_GET_PTR(CCDisplay*, m_pRenderDisplay, RenderDisplay);

	MEMBER_GET_CREF(float, m_fAlpha, Alpha);

	CCBaseData				m_obWorldInfo;
	MEMBER_GET_CREF(CCAffineTransform, m_obWorldTransform, WorldTransform);
};

} // namespace

#endif // _CC_BONE_H_