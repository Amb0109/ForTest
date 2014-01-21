#ifndef _CC_ARMATURE_H_
#define _CC_ARMATURE_H_

#include "../cocos/CCMacro.h"
#include "../cocos/CCNode.h"
#include "../cocos/CCAffineTransform.h"
#include "../data/CCData.h"
#include <map>

namespace CC {

class CCBone;
class CCAnimation;
class CCTextureAtlas;
class CCAtlasPage;
class CCDataCollection;
class CCArmatureData;
class CCAnimationData;

class CC_DLL CCArmature : public CCNode
{
public:
	typedef std::map<std::string, CCBone*> CC_BONE_MAP;

public:
	CCArmature();
	virtual ~CCArmature();

public:
	bool		init(CCArmatureData* armatureData);
	bool		initAnimation(CCAnimationData* animationData);
	bool		initTexture();
	void		release();
	CCBone*		createBone(const char* boneName);

	void		updateTransform(float delta);
	void		updateAnimation(float delta);

	void		render();
	void		draw();

	CCTextureAtlas*	getTextureAtlas(int index = 0);
	CCTextureAtlas* getTextureAtlasByPage(CCAtlasPage* atlasPage);

protected:
	static bool	_boneSortCompareFunc(const CCBone* a, const CCBone* b);
	void _sortChilds();
	void _updateBoundaryRect(float *vertex);

private:
	MEMBER_GET_PTR(CCDataCollection*, m_pDataCollection, DataCollection);
	MEMBER_GET_PTR(CCArmatureData*, m_pData, Data);
	MEMBER_GET_PTR(CCAnimation*, m_pAnimation, Animation);

	std::vector<CCBone*>				m_sRootBonesList;
	MEMBER_GET_CREF(CC_BONE_MAP, m_sBonesMap, BonesMap);

	bool								m_bNeedReorder;
	std::vector<CCBone*>				m_sOrderedBonesList;

	std::vector<CCTextureAtlas*>		m_sTextureAtlas;
	std::map<CCAtlasPage*, int>			m_sTextureAtlasMap;

	MEMBER_GET_CREF(int, m_iDrawCallCount, DrawCallCount);
	MEMBER_GET_CREF(int, m_iVertexCount, VertexCount);
	MEMBER_GET_CREF(int, m_iTriangleCount, TriangleCount);
	MEMBER_GET_CREF(CCRect, m_obBoundaryRect, BoundaryRect);

	MEMBER_GET_CREF(std::vector<std::string>, m_sMovementNameList, MovementNameList);
	MEMBER_GET_CREF(int, m_iMovementCount, MovementCount);

	float								m_fAlpha;
};

} // namespace

#endif // _CC_ARMATURE_H_