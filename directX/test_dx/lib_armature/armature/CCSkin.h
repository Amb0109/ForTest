#ifndef _CC_SKIN_H_
#define _CC_SKIN_H_

#include "../cocos/CCNode.h"
#include "../cocos/CCAffineTransform.h"
#include "../cocos/CCMacro.h"
#include <string>

namespace CC {

class CCBaseData;
class CCArmature;
class CCBone;
class CCDisplayData;
struct CCAtlasData;
class CCTextureData;
class CCDataCollection;
class CCTextureAtlas;

class CC_DLL CCSkin : public CCNode
{
public:
	CCSkin();
	virtual ~CCSkin();

public:
	bool init(CCDisplayData* displayData, CCBone* bone);
	bool initTextureData(const char* textureName);
	bool initAtlasData(const char* atlasName = NULL);
	void release();

	void update(float delta);
	void updateTransform();
	CCAffineTransform nodeToParentTransform(void);

	float*	getVerties();
	float*	getUVs();

private:
	MEMBER_GET_PTR(CCArmature*, m_pArmature, Armature);
	MEMBER_GET_PTR(CCBone*, m_pBone, Bone);
	MEMBER_GET_PTR(CCAtlasData*, m_pAtlasData, AtlasData);
	MEMBER_GET_PTR(CCTextureData*, m_pTextureData, TextureData);
	MEMBER_GET_PTR(CCDataCollection*, m_pDataCollection, DataCollection);
	MEMBER_GET_PTR(CCTextureAtlas*, m_pTextureAtlas, TextureAtlas);

	CCAffineTransform	m_obSkinTransform;

	std::string			m_sAtlasName;
	float				m_arrVerties[8];
	float				m_arrUVs[8];
	MEMBER_GET_CREF(float, m_fAlpha, Alpha);

	CCRect				m_obRect;
	CCPoint				m_obOffsetPosition;
};

} // namespace

#endif // _CC_SKIN_H_