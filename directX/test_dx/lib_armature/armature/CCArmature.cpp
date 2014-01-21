#include "CCArmature.h"

#include "../cocos/CCTransformHelp.h"
#include "../data/CCArmatureData.h"
#include "../data/CCAnimationData.h"
#include "../armature/CCBone.h"
#include "../armature/CCDisplay.h"
#include "../armature/CCSkin.h"
#include "../armature/CCTextureAtlas.h"
#include "../animation/CCAnimation.h"

#include <algorithm>

namespace CC {

CCArmature::CCArmature()
: m_pDataCollection(NULL)
, m_pData(NULL)
, m_pAnimation(NULL)
, m_sRootBonesList()
, m_sOrderedBonesList()
, m_sBonesMap()
, m_bNeedReorder(true)
, m_sTextureAtlas()
, m_sTextureAtlasMap()
, m_iDrawCallCount(0)
, m_iVertexCount(0)
, m_iTriangleCount(0)
, m_obBoundaryRect(CCRectZero)
{
}

CCArmature::~CCArmature()
{
	release();
}

void CCArmature::release()
{
	m_pDataCollection = NULL;
	m_pData = NULL;

	FOR_EACH (CC_BONE_MAP, m_sBonesMap, it)
	{
		if (CCBone* pBone = dynamic_cast<CCBone*>(it->second))
		{
			pBone->release();
			delete pBone;
		}
	}

	m_sRootBonesList.clear();
	m_sOrderedBonesList.clear();
	m_sBonesMap.clear();
	m_bNeedReorder = true;

	FOR_EACH (std::vector<CCTextureAtlas*>, m_sTextureAtlas, it)
	{
		if (CCTextureAtlas* pTextureAtlas = dynamic_cast<CCTextureAtlas*>(*it))
		{
			pTextureAtlas->release();
			delete pTextureAtlas;
		}
	}
	m_sTextureAtlas.clear();
	m_sTextureAtlasMap.clear();

	m_iDrawCallCount = 0;
	m_iVertexCount = 0;
	m_iTriangleCount = 0;
}

bool CCArmature::init( CCArmatureData* armatureData )
{
	if(armatureData == NULL) return false;
	m_pData = armatureData;
	m_pDataCollection = m_pData->getDataCollection();

	for (int i=0; ; ++i)
	{
		CCBoneData* pBoneData = m_pData->getBoneData(i);
		if (pBoneData == NULL) break;

		createBone(pBoneData->m_sName.c_str());
	}

	FOR_EACH (CC_BONE_MAP, m_sBonesMap, it)
	{
		if (CCBone *pBone = dynamic_cast<CCBone *>(it->second))
		{
			m_sOrderedBonesList.push_back(pBone);
		}
	}

	return true;
}

bool CCArmature::initAnimation( CCAnimationData* pAnimationData )
{
	if (pAnimationData == NULL) return false;
	
	m_pAnimation = new CCAnimation();
	if (m_pAnimation == NULL) return false;
	m_pAnimation->init(pAnimationData, this);

	for (unsigned i=0; ; ++i)
	{
		if(CCMovData* pMovData = pAnimationData->getMovementData(i))
		{
			m_sMovementNameList.push_back(pMovData->getName());
		}
		else break;
	}
	m_iMovementCount = (int)m_sMovementNameList.size();

	CCMovData *pMovData = pAnimationData->getMovementData();
	if (pMovData == NULL) return false;

	FOR_EACH (CC_BONE_MAP, m_sBonesMap, it)
	{
		if (CCBone *pBone = dynamic_cast<CCBone *>(it->second))
		{
			CCBoneData* pBoneData = pBone->getData();
			if (pBoneData == NULL)
			{
				pBone->initAnimation(NULL);
			}
			else
			{
				pBone->initAnimation(pMovData->getMovementBoneDataByName(pBoneData->m_sName.c_str()));
			}
		}
	}
	return true;
}

bool CCArmature::initTexture()
{
	if (m_pDataCollection == NULL) return false;

	for (int i=0; ; ++i)
	{
		CCAtlasPage* pAtlasPage = m_pDataCollection->getAtlasPage(i);
		if(pAtlasPage == NULL) break;

		CCTextureAtlas* pTextureAtlas = new CCTextureAtlas();
		m_sTextureAtlas.push_back(pTextureAtlas);
		m_sTextureAtlasMap[pAtlasPage] = i;
		if (pTextureAtlas == NULL) continue;
		pTextureAtlas->init(pAtlasPage);
	}
	return true;
}

CCBone* CCArmature::createBone( const char* boneName )
{
	if (m_pData == NULL) return NULL;
	if (boneName == NULL) return NULL;

	CC_BONE_MAP::iterator itorExistBone = m_sBonesMap.find(boneName);
	if(itorExistBone != m_sBonesMap.end()) return itorExistBone->second;

	CCBoneData* pBoneData = m_pData->getBoneDataByName(boneName);
	if (pBoneData == NULL) return NULL;

	CCBone* pBone = new CCBone();
	if (pBone == NULL) return NULL;
	pBone->init(pBoneData, this);

	if (pBoneData->m_sParent.length() == 0)
	{ // root
		m_sRootBonesList.push_back(pBone);
	}
	else
	{
		CCBone* pParentBone = createBone(pBoneData->m_sParent.c_str());
		if (pParentBone == NULL)
		{
			delete pBone;
			pBone = NULL;
			return NULL;
		}

		pParentBone->addChild(pBone);
	}

	m_sBonesMap[pBoneData->m_sName] = pBone;
	return pBone;
}

void CCArmature::updateTransform( float delta )
{
	FOR_EACH (std::vector<CCBone*>, m_sRootBonesList, it)
	{
		if (CCBone* pBone = dynamic_cast<CCBone*>(*it))
		{
			pBone->update(delta);
		}
	}
}

void CCArmature::updateAnimation( float delta )
{
	if (m_pAnimation == NULL) return;
	m_pAnimation->update(delta);
}

void CCArmature::render()
{
	_sortChilds();
	draw();
}

void CCArmature::_sortChilds()
{
	if (!m_bNeedReorder) return;
	std::sort(m_sOrderedBonesList.begin(), m_sOrderedBonesList.end(),
		CCArmature::_boneSortCompareFunc);
	m_bNeedReorder = false;
}

void CCArmature::draw()
{
	CCTextureAtlas* pCurrentTextureAtlas = NULL;
	CC_PAINT_QUAD quad;

	m_iDrawCallCount = 0;
	m_iVertexCount = 0;
	m_iTriangleCount = 0;

	m_obBoundaryRect = CCRectZero;

	FOR_EACH (std::vector<CCBone*>, m_sOrderedBonesList, it)
	{
		if (CCBone *pBone = dynamic_cast<CCBone *>(*it))
		{
			CCDisplay* pDisplay = pBone->getRenderDisplay();
			if (pDisplay == NULL) continue;
			switch(pDisplay->get_type())
			{
			case CC_DISPLAY_SPRITE:
				{
					CCSkin* pSkin = pDisplay->getSkin();
					if (pSkin != NULL)
					{
						CCTextureAtlas* pTextureAtlas = pSkin->getTextureAtlas();
						if (pTextureAtlas == NULL)
						{
							pSkin->initAtlasData();
							pTextureAtlas = pSkin->getTextureAtlas();
							if (pTextureAtlas == NULL) continue;
						}
						
						if(pCurrentTextureAtlas != NULL && pCurrentTextureAtlas != pTextureAtlas)
						{
							++ m_iDrawCallCount;
							pCurrentTextureAtlas->drawQuads();
							pCurrentTextureAtlas->clearQuads();
							pCurrentTextureAtlas = pTextureAtlas;
							pCurrentTextureAtlas->clearQuads();
						}
						else if (pCurrentTextureAtlas == NULL)
						{
							pCurrentTextureAtlas = pTextureAtlas;
							pCurrentTextureAtlas->clearQuads();
						}

						if(pSkin->isVisible())
						{
							pSkin->updateTransform();
							float* vtx = pSkin->getVerties();
							float* uv = pSkin->getUVs();
							unsigned alpha = (unsigned)(0xff * (pSkin->getAlpha() * m_fAlpha));
							unsigned color =  (alpha << 24) | 0xffffffff;

							quad.tl = CC_PAINT_VERTEX(vtx[0], vtx[1], 0.f, color, uv[0], uv[1]);
							quad.tr = CC_PAINT_VERTEX(vtx[2], vtx[3], 0.f, color, uv[2], uv[3]);
							quad.br = CC_PAINT_VERTEX(vtx[4], vtx[5], 0.f, color, uv[4], uv[5]);
							quad.bl = CC_PAINT_VERTEX(vtx[6], vtx[7], 0.f, color, uv[6], uv[7]);

							pCurrentTextureAtlas->addQuad(quad);

							m_iVertexCount += 4;
							m_iTriangleCount += 2;
							//_updateBoundaryRect(vtx);
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}

	if (pCurrentTextureAtlas)
	{
		++ m_iDrawCallCount;
		pCurrentTextureAtlas->drawQuads();
		pCurrentTextureAtlas->clearQuads();
	}
}

bool CCArmature::_boneSortCompareFunc( const CCBone* a, const CCBone* b )
{
	return a->getDrawOrder() < b->getDrawOrder();
}

CCTextureAtlas* CCArmature::getTextureAtlas( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_sTextureAtlas.size() <= (unsigned)index) return NULL;
	return m_sTextureAtlas[(unsigned)index];
}

CCTextureAtlas* CCArmature::getTextureAtlasByPage( CCAtlasPage* atlasPage )
{
	if (m_sTextureAtlasMap.find(atlasPage) == m_sTextureAtlasMap.end()) return NULL;
	return getTextureAtlas(m_sTextureAtlasMap[atlasPage]);
}

void CCArmature::_updateBoundaryRect( float *vertex )
{
	if (m_obBoundaryRect.equals(CCRectZero))
	{
		m_obBoundaryRect.origin.x = vertex[0];
		m_obBoundaryRect.origin.y = vertex[1];
	}
	for (int i=0; i<8; i+=2)
	{
		float lx = m_obBoundaryRect.getMinX();
		float rx = m_obBoundaryRect.getMaxX();
		float ly = m_obBoundaryRect.getMinY();
		float ry = m_obBoundaryRect.getMaxY();

		if (vertex[i] < lx)
		{
			m_obBoundaryRect.origin.x = vertex[i];
			m_obBoundaryRect.size.width = rx - vertex[i];
		}
		else if (rx < vertex[i])
		{
			m_obBoundaryRect.size.width = vertex[i] - lx;
		}

		if (vertex[i+1] < ly)
		{
			m_obBoundaryRect.origin.y = vertex[i+1];
			m_obBoundaryRect.size.height = ry - m_obBoundaryRect.origin.y;
		}
		else if (ry < vertex[i+1])
		{
			m_obBoundaryRect.size.height = vertex[i+1] - ly;
		}
	}
}

} // namespace