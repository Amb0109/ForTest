#include "CCNode.h"

namespace CC {

CCNode::CCNode()
: m_obPosition(CCPointZero)
, m_fScaleX(1.f)
, m_fScaleY(1.f)
, m_fRotationX(0.f)
, m_fRotationY(0.f)
, m_fSkewX(0.f)
, m_fSkewY(0.f)
, m_fVertexZ(0.f)
, m_bVisible(true)
//, m_fAlpha(1.f)
//, m_bFlipX(false)
//, m_bFlipY(false)
, m_sTransform(CCAffineTransformIdentity)
//, m_sAdditionalTransform(CCAffineTransformIdentity)
, m_sInverse(CCAffineTransformIdentity)
{

}

CCNode::~CCNode()
{

}

float CCNode::getSkewX()
{
	return m_fSkewX;
}

void CCNode::setSkewX(float newSkewX)
{
	m_fSkewX = newSkewX;
	m_bTransformDirty = m_bInverseDirty = true;
}

float CCNode::getSkewY()
{
	return m_fSkewY;
}

void CCNode::setSkewY(float newSkewY)
{
	m_fSkewY = newSkewY;

	m_bTransformDirty = m_bInverseDirty = true;
}

/// zOrder getter
int CCNode::getZOrder()
{
	return m_nZOrder;
}

/// zOrder setter : private method
/// used internally to alter the zOrder variable. DON'T call this method manually 
void CCNode::_setZOrder(int z)
{
	m_nZOrder = z;
}

void CCNode::setZOrder(int z)
{
	_setZOrder(z);
	//if (m_pParent)
	//{
	//	m_pParent->reorderChild(this, z);
	//}
}

/// vertexZ getter
float CCNode::getVertexZ()
{
	return m_fVertexZ;
}


/// vertexZ setter
void CCNode::setVertexZ(float var)
{
	m_fVertexZ = var;
}


/// rotation getter
float CCNode::getRotation()
{
	//CCAssert(m_fRotationX == m_fRotationY, "CCSprite#rotation. RotationX != RotationY. Don't know which one to return");
	return m_fRotationX;
}

/// rotation setter
void CCNode::setRotation(float newRotation)
{
	if (newRotation == m_fRotationX && newRotation == m_fRotationY) return;
	m_fRotationX = m_fRotationY = newRotation;
	m_bTransformDirty = m_bInverseDirty = true;
}

float CCNode::getRotationX()
{
	return m_fRotationX;
}

void CCNode::setRotationX(float fRotationX)
{
	if (fRotationX == m_fRotationX) return;
	m_fRotationX = fRotationX;
	m_bTransformDirty = m_bInverseDirty = true;
}

float CCNode::getRotationY()
{
	return m_fRotationY;
}

void CCNode::setRotationY(float fRotationY)
{
	if (fRotationY == m_fRotationY) return;
	m_fRotationY = fRotationY;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scale getter
float CCNode::getScale(void)
{
	//CCAssert( m_fScaleX == m_fScaleY, "CCSprite#scale. ScaleX != ScaleY. Don't know which one to return");
	return m_fScaleX;
}

/// scale setter
void CCNode::setScale(float scale)
{
	if (m_fScaleX == scale && m_fScaleY == scale) return;
	m_fScaleX = m_fScaleY = scale;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scale setter
void CCNode::setScale(float fScaleX, float fScaleY)
{
	if (m_fScaleX == fScaleX && m_fScaleY == fScaleY) return;
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleX getter
float CCNode::getScaleX()
{
	return m_fScaleX;
}

/// scaleX setter
void CCNode::setScaleX(float newScaleX)
{
	if (m_fScaleX == newScaleX) return;
	m_fScaleX = newScaleX;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleY getter
float CCNode::getScaleY()
{
	return m_fScaleY;
}

/// scaleY setter
void CCNode::setScaleY(float newScaleY)
{
	if (m_fScaleY == newScaleY) return;
	m_fScaleY = newScaleY;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// position getter
const CCPoint& CCNode::getPosition()
{
	return m_obPosition;
}

/// position setter
void CCNode::setPosition(const CCPoint& newPosition)
{
	if (m_obPosition.equals(newPosition)) return;
	m_obPosition = newPosition;
	m_bTransformDirty = m_bInverseDirty = true;
}

void CCNode::getPosition(float* x, float* y)
{
	*x = m_obPosition.x;
	*y = m_obPosition.y;
}

void CCNode::setPosition(float x, float y)
{
	setPosition(CCPointMake(x, y));
}

float CCNode::getPositionX(void)
{
	return m_obPosition.x;
}

float CCNode::getPositionY(void)
{
	return  m_obPosition.y;
}

void CCNode::setPositionX(float x)
{
	setPosition(CCPointMake(x, m_obPosition.y));
}

void CCNode::setPositionY(float y)
{
	setPosition(CCPointMake(m_obPosition.x, y));
}

/// isVisible getter
bool CCNode::isVisible()
{
	return m_bVisible;
}

/// isVisible setter
void CCNode::setVisible(bool var)
{
	m_bVisible = var;
}

/// contentSize getter
const CCSize& CCNode::getContentSize() const
{
	return m_obContentSize;
}

void CCNode::setContentSize(const CCSize & size)
{
	if ( ! size.equals(m_obContentSize))
	{
		m_obContentSize = size;

		m_obAnchorPointInPoints = CCPointMake(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
		m_bTransformDirty = m_bInverseDirty = true;
	}
}

const CCPoint& CCNode::getAnchorPointInPoints()
{
	return m_obAnchorPointInPoints;
}

/// anchorPoint getter
const CCPoint& CCNode::getAnchorPoint()
{
	return m_obAnchorPoint;
}

void CCNode::setAnchorPoint(const CCPoint& point)
{
	if( ! point.equals(m_obAnchorPoint))
	{
		m_obAnchorPoint = point;
		m_obAnchorPointInPoints = CCPointMake(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
		m_bTransformDirty = m_bInverseDirty = true;
	}
}
//
//void CCSprite::setAlpha( float fAlpha )
//{
//	m_fAlpha = fAlpha;
//}
//
//float CCSprite::getAlpha()
//{
//	return m_fAlpha;
//}


//bool CCSprite::isFlipX( void )
//{
//	return m_bFlipX;
//}
//
//void CCSprite::setFlipX( bool bFlipX )
//{
//	if (bFlipX == m_bFlipX) return;
//	m_bFlipX = bFlipX;
//	m_bTransformDirty = m_bInverseDirty = true;
//}
//
//bool CCSprite::isFlipY( void )
//{
//	return m_bFlipY;
//}
//
//void CCSprite::setFlipY( bool bFlipY )
//{
//	if (bFlipY == m_bFlipY) return;
//	m_bFlipY = bFlipY;
//	m_bTransformDirty = m_bInverseDirty = true;
//}
//

} // namespace