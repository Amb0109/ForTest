#ifndef _CC_DISPLAY_H_
#define _CC_DISPLAY_H_

#include "../cocos/CCMacro.h"

namespace CC {

class CCDisplayData;
class CCBone;
class CCSkin;
enum CCDisplayType;

class CC_DLL CCDisplay
{
public:
	CCDisplay();
	virtual ~CCDisplay();

public:
	bool init(CCDisplayData* data, CCBone* bone);
	void release();

	virtual void update(float delta);

	CCDisplayType get_type();

private:
	MEMBER_GET_PTR(CCDisplayData*, m_pData, Data);
	MEMBER_GET_PTR(CCBone*, m_pBone, Bone);
	MEMBER_GET_PTR(CCSkin*, m_pSkin, Skin);

	CCDisplayType		m_type;
};

} // namespace

#endif // _CC_DISPLAY_H_