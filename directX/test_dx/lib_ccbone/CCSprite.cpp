#include "CCSprite.h"


float CCSprite::getSkewX()
{
	return m_fSkewX;
}

void CCSprite::setSkewX(float newSkewX)
{
	m_fSkewX = newSkewX;
	m_bTransformDirty = m_bInverseDirty = true;
}

float CCSprite::getSkewY()
{
	return m_fSkewY;
}

void CCSprite::setSkewY(float newSkewY)
{
	m_fSkewY = newSkewY;

	m_bTransformDirty = m_bInverseDirty = true;
}

/// zOrder getter
int CCSprite::getZOrder()
{
	return m_nZOrder;
}

/// zOrder setter : private method
/// used internally to alter the zOrder variable. DON'T call this method manually 
void CCSprite::_setZOrder(int z)
{
	m_nZOrder = z;
}

void CCSprite::setZOrder(int z)
{
	_setZOrder(z);
	//if (m_pParent)
	//{
	//	m_pParent->reorderChild(this, z);
	//}
}

/// vertexZ getter
float CCSprite::getVertexZ()
{
	return m_fVertexZ;
}


/// vertexZ setter
void CCSprite::setVertexZ(float var)
{
	m_fVertexZ = var;
}


/// rotation getter
float CCSprite::getRotation()
{
	//CCAssert(m_fRotationX == m_fRotationY, "CCSprite#rotation. RotationX != RotationY. Don't know which one to return");
	return m_fRotationX;
}

/// rotation setter
void CCSprite::setRotation(float newRotation)
{
	m_fRotationX = m_fRotationY = newRotation;
	m_bTransformDirty = m_bInverseDirty = true;
}

float CCSprite::getRotationX()
{
	return m_fRotationX;
}

void CCSprite::setRotationX(float fRotationX)
{
	m_fRotationX = fRotationX;
	m_bTransformDirty = m_bInverseDirty = true;
}

float CCSprite::getRotationY()
{
	return m_fRotationY;
}

void CCSprite::setRotationY(float fRotationY)
{
	m_fRotationY = fRotationY;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scale getter
float CCSprite::getScale(void)
{
	//CCAssert( m_fScaleX == m_fScaleY, "CCSprite#scale. ScaleX != ScaleY. Don't know which one to return");
	return m_fScaleX;
}

/// scale setter
void CCSprite::setScale(float scale)
{
	m_fScaleX = m_fScaleY = scale;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scale setter
void CCSprite::setScale(float fScaleX,float fScaleY)
{
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleX getter
float CCSprite::getScaleX()
{
	return m_fScaleX;
}

/// scaleX setter
void CCSprite::setScaleX(float newScaleX)
{
	m_fScaleX = newScaleX;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleY getter
float CCSprite::getScaleY()
{
	return m_fScaleY;
}

/// scaleY setter
void CCSprite::setScaleY(float newScaleY)
{
	m_fScaleY = newScaleY;
	m_bTransformDirty = m_bInverseDirty = true;
}

/// position getter
const CCPoint& CCSprite::getPosition()
{
	return m_obPosition;
}

/// position setter
void CCSprite::setPosition(const CCPoint& newPosition)
{
	m_obPosition = newPosition;
	m_bTransformDirty = m_bInverseDirty = true;
}

void CCSprite::getPosition(float* x, float* y)
{
	*x = m_obPosition.x;
	*y = m_obPosition.y;
}

void CCSprite::setPosition(float x, float y)
{
	setPosition(CCPointMake(x, y));
}

float CCSprite::getPositionX(void)
{
	return m_obPosition.x;
}

float CCSprite::getPositionY(void)
{
	return  m_obPosition.y;
}

void CCSprite::setPositionX(float x)
{
	setPosition(CCPointMake(x, m_obPosition.y));
}

void CCSprite::setPositionY(float y)
{
	setPosition(CCPointMake(m_obPosition.x, y));
}

/// isVisible getter
bool CCSprite::isVisible()
{
	return m_bVisible;
}

/// isVisible setter
void CCSprite::setVisible(bool var)
{
	m_bVisible = var;
}

/// contentSize getter
const CCSize& CCSprite::getContentSize() const
{
	return m_obContentSize;
}

void CCSprite::setContentSize(const CCSize & size)
{
	if ( ! size.equals(m_obContentSize))
	{
		m_obContentSize = size;

		m_obAnchorPointInPoints = CCPointMake(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
		m_bTransformDirty = m_bInverseDirty = true;
	}
}

const CCPoint& CCSprite::getAnchorPointInPoints()
{
	return m_obAnchorPointInPoints;
}

/// anchorPoint getter
const CCPoint& CCSprite::getAnchorPoint()
{
	return m_obAnchorPoint;
}

void CCSprite::setAnchorPoint(const CCPoint& point)
{
	if( ! point.equals(m_obAnchorPoint))
	{
		m_obAnchorPoint = point;
		m_obAnchorPointInPoints = CCPointMake(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
		m_bTransformDirty = m_bInverseDirty = true;
	}
}

CCAffineTransform CCSprite::nodeToParentTransform(void)
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

		if (m_bAdditionalTransformDirty)
		{
			m_sTransform = CCAffineTransformConcat(m_sTransform, m_sAdditionalTransform);
			m_bAdditionalTransformDirty = false;
		}

		m_bTransformDirty = false;
	}

	return m_sTransform;
}


