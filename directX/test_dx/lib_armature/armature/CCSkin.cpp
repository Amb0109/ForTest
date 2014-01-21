#include "CCSkin.h"
#include "../data/CCData.h"
#include "CCBone.h"
#include "CCArmature.h"
#include "../data/CCArmatureData.h"
#include "../data/CCAtlasData.h"
#include "../data/CCTextureData.h"
#include "CCTextureAtlas.h"
#include <assert.h>

namespace CC {

CCSkin::CCSkin()
: m_pAtlasData(NULL)
, m_pTextureData(NULL)
, m_pDataCollection(NULL)
, m_pTextureAtlas(NULL)
, m_obSkinTransform(CCAffineTransformIdentity)
{
}

CCSkin::~CCSkin()
{
	release();
}

void CCSkin::release()
{
	m_pAtlasData = NULL;
	m_pTextureData = NULL;
	m_pDataCollection = NULL;
	m_pTextureAtlas = NULL;
	m_obSkinTransform = CCAffineTransformIdentity;

	m_sAtlasName.clear();
}

bool CCSkin::init( CCDisplayData* displayData, CCBone* bone )
{
	if (displayData == NULL) return false;
	if (bone == NULL) return false;

	m_pBone = bone;
	m_pArmature = bone->getArmature();
	m_sAtlasName = displayData->getName();
	CCBaseData* pSkinData = displayData->getSkinData();
	if (pSkinData == NULL) return false;

	setScaleX(pSkinData->scaleX);
	setScaleY(pSkinData->scaleY);
	setRotationX(CC_RADIANS_TO_DEGREES(pSkinData->skewX));
	setRotationY(CC_RADIANS_TO_DEGREES(-pSkinData->skewY));
	setPosition(CCPointMake(pSkinData->x, pSkinData->y));

	if (!m_pArmature) return false;
	
	m_pDataCollection = m_pArmature->getDataCollection();
	if (m_pDataCollection == NULL) return false;

	return true;
}

void CCSkin::update( float delta )
{
	if (m_pBone == NULL) return;
	if (m_pBone->isTransformDirty())
		m_sTransform = CCAffineTransformConcat(m_obSkinTransform, m_pBone->getWorldTransform());
	m_fAlpha = m_pBone->getAlpha();
}

void CCSkin::updateTransform()
{
	if( !m_bVisible)
	{
		memset(m_arrVerties, 0, sizeof(float) * 8);
	}
	else
	{
		CCSize &size = m_obRect.size;

		float x1 = m_obOffsetPosition.x;
		float y1 = m_obOffsetPosition.y;

		float x2 = x1 + size.width;
		float y2 = y1 + size.height;

		float x = m_sTransform.tx;
		float y = m_sTransform.ty;

		float cr = m_sTransform.a;
		float sr = m_sTransform.b;
		float cr2 = m_sTransform.d;
		float sr2 = -m_sTransform.c;
		float ax = x1 * cr - y1 * sr2 + x;
		float ay = x1 * sr + y1 * cr2 + y;

		float bx = x2 * cr - y1 * sr2 + x;
		float by = x2 * sr + y1 * cr2 + y;

		float cx = x2 * cr - y2 * sr2 + x;
		float cy = x2 * sr + y2 * cr2 + y;

		float dx = x1 * cr - y2 * sr2 + x;
		float dy = x1 * sr + y2 * cr2 + y;

		m_arrVerties[0] = ax; m_arrVerties[1] = ay;
		m_arrVerties[2] = bx; m_arrVerties[3] = by;
		m_arrVerties[4] = cx; m_arrVerties[5] = cy;
		m_arrVerties[6] = dx; m_arrVerties[7] = dy;

		if (m_pArmature)
		{
			//if (m_armature->isFlipX())
			//{
			//	float base_2x = 2.f * m_armature->getPositionX();
			//	for (int i=0; i<8; i+=2)
			//	{
			//		m_verties[i] = base_2x - m_verties[i];
			//	}
			//}

			//if (!m_armature->isFlipY())
			//{
			//	float base_2y = 2.f * m_armature->getPositionY();
			//	for (int i=1; i<8; i+=2)
			//	{
			//		m_verties[i] = base_2y - m_verties[i];
			//	}
			//}
		}
	}
}

float* CCSkin::getVerties()
{
	return m_arrVerties;
}

float* CCSkin::getUVs()
{
	return m_arrUVs;
}

bool CCSkin::initTextureData( const char* textureName )
{
	if (textureName == NULL || textureName[0] == '\0') return false;
	if (m_pDataCollection == NULL) return false;
	
	m_pTextureData = m_pDataCollection->getTextureDataByName(textureName);
	if (m_pTextureData == NULL)
	{
		assert(false);
		return false;
	}

	setAnchorPoint(CCPointMake(m_pTextureData->m_fPivotX, m_pTextureData->m_fPivotY));
	
	m_obSkinTransform = nodeToParentTransform();
	m_sTransform = CCAffineTransformConcat(m_obSkinTransform, m_pBone->getWorldTransform());

	return true;
}

bool CCSkin::initAtlasData( const char* atlasName/* = NULL*/ )
{
	if (atlasName == NULL)
		return initAtlasData(m_sAtlasName.c_str());

	if (atlasName == NULL || atlasName[0] == '\0') return false;
	if (m_pDataCollection == NULL) return false;
	if (m_pBone == NULL) return false;

	CCArmature* pArmature = m_pBone->getArmature();
	if (pArmature == NULL) return false;

	m_pAtlasData = m_pDataCollection->getAtlasData(atlasName);
	if (m_pAtlasData == NULL)
	{
		assert(false);
		return false;
	}

	m_obRect = CCRectMake(m_pAtlasData->m_iX, m_pAtlasData->m_iY, m_pAtlasData->m_iWidth, m_pAtlasData->m_iHeight);
	m_obOffsetPosition = CCPointMake(m_pAtlasData->m_fOffsetX, m_pAtlasData->m_fOffsetY);
	m_obContentSize = CCSizeMake(m_pAtlasData->m_iWidth, m_pAtlasData->m_iHeight);

	CCAtlasPage* pAtlasPage = m_pAtlasData->m_pPage;
	if (pAtlasPage == NULL)
	{
		assert(false);
		return false;
	}

	m_pTextureAtlas = pArmature->getTextureAtlasByPage(pAtlasPage);
	if (m_pTextureAtlas == NULL)
	{
		assert(false);
		return false;
	}

	CCSize textureSize = m_pTextureAtlas->getTextureSize();
	if (textureSize.width == 0 || textureSize.height == 0) return false;
	m_arrUVs[0] = m_obRect.getMinX() / textureSize.width;
	m_arrUVs[1] = m_obRect.getMaxY() / textureSize.height;
	m_arrUVs[2] = m_obRect.getMaxX() / textureSize.width;
	m_arrUVs[3] = m_obRect.getMaxY() / textureSize.height;
	m_arrUVs[4] = m_obRect.getMaxX() / textureSize.width;
	m_arrUVs[5] = m_obRect.getMinY() / textureSize.height;
	m_arrUVs[6] = m_obRect.getMinX() / textureSize.width;
	m_arrUVs[7] = m_obRect.getMinY() / textureSize.height;

	std::string sTextureName = atlasName;
	size_t startPos = sTextureName.find_last_of(".");
	if(startPos != std::string::npos)
	{
		sTextureName = sTextureName.erase(startPos);
	}

	return initTextureData(sTextureName.c_str());
}

CCAffineTransform CCSkin::nodeToParentTransform(void)
{
	if (m_bTransformDirty) 
	{
		// Translate values
		float x = m_obPosition.x;
		float y = m_obPosition.y;
		
		//if (m_bIgnoreAnchorPointForPosition) 
		//{
		//	x += m_obAnchorPointInPoints.x;
		//	y += m_obAnchorPointInPoints.y;
		//}

		// Rotation values
		// Change rotation code to handle X and Y
		// If we skew with the exact same value for both x and y then we're simply just rotating
		float cx = 1, sx = 0, cy = 1, sy = 0;
		if (m_fRotationX || m_fRotationY)
		{
			float radiansX = -CC_DEGREES_TO_RADIANS(m_fRotationX);
			float radiansY = -CC_DEGREES_TO_RADIANS(m_fRotationY);
			cx = cosf(radiansX);
			sx = sinf(radiansX);
			cy = cosf(radiansY);
			sy = sinf(radiansY);
		}

		bool needsSkewMatrix = ( m_fSkewX || m_fSkewY );


		// optimization:
		// inline anchor point calculation if skew is not needed
		// Adjusted transform calculation for rotational skew
		if (! needsSkewMatrix && !m_obAnchorPointInPoints.equals(CCPointZero))
		{
			x += cy * -m_obAnchorPointInPoints.x * m_fScaleX + -sx * -m_obAnchorPointInPoints.y * m_fScaleY;
			y += sy * -m_obAnchorPointInPoints.x * m_fScaleX +  cx * -m_obAnchorPointInPoints.y * m_fScaleY;
		}


		// Build Transform Matrix
		// Adjusted transform calculation for rotational skew
		m_sTransform = CCAffineTransformMake( cy * m_fScaleX,  sy * m_fScaleX,
			-sx * m_fScaleY, cx * m_fScaleY,
			x, y );

		// XXX: Try to inline skew
		// If skew is needed, apply skew and then anchor point
		if (needsSkewMatrix) 
		{
			CCAffineTransform skewMatrix = CCAffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
				tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
				0.0f, 0.0f );
			m_sTransform = CCAffineTransformConcat(skewMatrix, m_sTransform);

			// adjust anchor point
			if (!m_obAnchorPointInPoints.equals(CCPointZero))
			{
				m_sTransform = CCAffineTransformTranslate(m_sTransform, -m_obAnchorPointInPoints.x, -m_obAnchorPointInPoints.y);
			}
		}

		//if (m_bAdditionalTransformDirty)
		//{
		//	m_sTransform = CCAffineTransformConcat(m_sTransform, m_sAdditionalTransform);
		//	m_bAdditionalTransformDirty = false;
		//}

		m_bTransformDirty = false;
	}

	return m_sTransform;
}

} // namespace