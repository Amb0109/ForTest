#ifndef _CC_NODE_H_
#define _CC_NODE_H_

#include "../cocos/CCMacro.h"
#include "CCAffineTransform.h"

namespace CC {

class CC_DLL CCNode
{
public:
	CCNode();
	virtual ~CCNode();

public:

    /// @{
    /// @name Setters & Getters for Graphic Peroperties
    
    /**
     * Sets the Z order which stands for the drawing order, and reorder this node in its parent's children array.
     *
     * The Z order of node is relative to its "brothers": children of the same parent.
     * It's nothing to do with OpenGL's z vertex. This one only affects the draw order of nodes in cocos2d.
     * The larger number it is, the later this node will be drawn in each message loop.
     * Please refer to setVertexZ(float) for the difference.
     *
     * @param nZOrder   Z order of this node.
     */
    virtual void setZOrder(int zOrder);
    /**
     * Sets the z order which stands for the drawing order
     *
     * This is an internal method. Don't call it outside the framework.
     * The difference between setZOrder(int) and _setOrder(int) is:
     * - _setZOrder(int) is a pure setter for m_nZOrder memeber variable
     * - setZOrder(int) firstly changes m_nZOrder, then recorder this node in its parent's chilren array.
     */
    virtual void _setZOrder(int z);
    /**
     * Gets the Z order of this node.
     *
     * @see setZOrder(int)
     *
     * @return The Z order.
     */
    virtual int getZOrder();


    /**
     * Sets the real OpenGL Z vertex.
     *
     * Differences between openGL Z vertex and cocos2d Z order:
     * - OpenGL Z modifies the Z vertex, and not the Z order in the relation between parent-children
     * - OpenGL Z might require to set 2D projection
     * - cocos2d Z order works OK if all the nodes uses the same openGL Z vertex. eg: vertexZ = 0
     *
     * @warning Use it at your own risk since it might break the cocos2d parent-children z order
     *
     * @param fVertexZ  OpenGL Z vertex of this node.
     */
    virtual void setVertexZ(float vertexZ);
    /**
     * Gets OpenGL Z vertex of this node.
     *
     * @see setVertexZ(float)
     *
     * @return OpenGL Z vertex of this node
     */
    virtual float getVertexZ();


    /**
     * Changes the scale factor on X axis of this node
     *
     * The deafult value is 1.0 if you haven't changed it before
     *
     * @param fScaleX   The scale factor on X axis.
     */
    virtual void setScaleX(float fScaleX);
    /**
     * Returns the scale factor on X axis of this node
     *
     * @see setScaleX(float)
     *
     * @return The scale factor on X axis.
     */
    virtual float getScaleX();

    
    /**
     * Changes the scale factor on Y axis of this node
     *
     * The Default value is 1.0 if you haven't changed it before.
     *
     * @param fScaleY   The scale factor on Y axis.
     */
    virtual void setScaleY(float fScaleY);
    /**
     * Returns the scale factor on Y axis of this node
     *
     * @see setScaleY(float)
     *
     * @return The scale factor on Y axis. 
     */
    virtual float getScaleY();

    
    /**
     * Changes both X and Y scale factor of the node.
     *
     * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
     *
     * @param scale     The scale factor for both X and Y axis.
     */
    virtual void setScale(float scale);
    /**
     * Gets the scale factor of the node,  when X and Y have the same scale factor.
     *
     * @warning Assert when m_fScaleX != m_fScaleY.
     * @see setScale(float)
     *
     * @return The scale factor of the node.
     */
    virtual float getScale();
    

    /**
     * Changes both X and Y scale factor of the node.
     *
     * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
     *
     * @param fScaleX     The scale factor on X axis.
     * @param fScaleY     The scale factor on Y axis.
     */
    virtual void setScale(float fScaleX,float fScaleY);

    
    /**
     * Changes the position (x,y) of the node in OpenGL coordinates
     *
     * Usually we use ccp(x,y) to compose CCPoint object.
     * The original point (0,0) is at the left-bottom corner of screen.
     * For example, this codesnip sets the node in the center of screen.
     * @code
     * CCSize size = CCDirector::sharedDirector()->getWinSize();
     * node->setPosition( ccp(size.width/2, size.height/2) )
     * @endcode
     *
     * @param position  The position (x,y) of the node in OpenGL coordinates
     * @js NA
     */
    virtual void setPosition(const CCPoint &position);
    /**
     * Gets the position (x,y) of the node in OpenGL coordinates
     * 
     * @see setPosition(const CCPoint&)
     *
     * @return The position (x,y) of the node in OpenGL coordinates
     */
    virtual const CCPoint& getPosition();
    /**
     * Sets position in a more efficient way.
     *
     * Passing two numbers (x,y) is much efficient than passing CCPoint object.
     * This method is binded to lua and javascript. 
     * Passing a number is 10 times faster than passing a object from lua to c++
     *
     * @code
     * // sample code in lua
     * local pos  = node::getPosition()  -- returns CCPoint object from C++
     * node:setPosition(x, y)            -- pass x, y coordinate to C++
     * @endcode
     *
     * @param x     X coordinate for position
     * @param y     Y coordinate for position
     * @js NA
     */
    virtual void setPosition(float x, float y);
    /**
     * Gets position in a more efficient way, returns two number instead of a CCPoint object
     *
     * @see setPosition(float, float)
     */
    virtual void getPosition(float* x, float* y);
    /**
     * Gets/Sets x or y coordinate individually for position.
     * These methods are used in Lua and Javascript Bindings
     */
    virtual void  setPositionX(float x);
    virtual float getPositionX(void);
    virtual void  setPositionY(float y);
    virtual float getPositionY(void);
    
    
    /**
     * Changes the X skew angle of the node in degrees.
     *
     * This angle describes the shear distortion in the X direction.
     * Thus, it is the angle between the Y axis and the left edge of the shape
     * The default skewX angle is 0. Positive values distort the node in a CW direction.
     *
     * @param fSkewX The X skew angle of the node in degrees.
     */
    virtual void setSkewX(float fSkewX);
    /**
     * Returns the X skew angle of the node in degrees.
     *
     * @see setSkewX(float)
     *
     * @return The X skew angle of the node in degrees.
     */
    virtual float getSkewX();

    
    /**
     * Changes the Y skew angle of the node in degrees.
     *
     * This angle describes the shear distortion in the Y direction.
     * Thus, it is the angle between the X axis and the bottom edge of the shape
     * The default skewY angle is 0. Positive values distort the node in a CCW direction.
     *
     * @param fSkewY    The Y skew angle of the node in degrees.
     */
    virtual void setSkewY(float fSkewY);
    /**
     * Returns the Y skew angle of the node in degrees.
     *
     * @see setSkewY(float)
     *
     * @return The Y skew angle of the node in degrees.
     */
    virtual float getSkewY();

    
    /**
     * Sets the anchor point in percent.
     *
     * anchorPoint is the point around which all transformations and positioning manipulations take place.
     * It's like a pin in the node where it is "attached" to its parent.
     * The anchorPoint is normalized, like a percentage. (0,0) means the bottom-left corner and (1,1) means the top-right corner.
     * But you can use values higher than (1,1) and lower than (0,0) too.
     * The default anchorPoint is (0.5,0.5), so it starts in the center of the node.
     *
     * @param anchorPoint   The anchor point of node.
     */
    virtual void setAnchorPoint(const CCPoint& anchorPoint);
    /** 
     * Returns the anchor point in percent.
     *
     * @see setAnchorPoint(const CCPoint&)
     *
     * @return The anchor point of node.
     */
    virtual const CCPoint& getAnchorPoint();
    /**
     * Returns the anchorPoint in absolute pixels.
     * 
     * @warning You can only read it. If you wish to modify it, use anchorPoint instead.
     * @see getAnchorPoint()
     *
     * @return The anchor point in absolute pixels.
     */
    virtual const CCPoint& getAnchorPointInPoints();
        

    /**
     * Sets the untransformed size of the node.
     *
     * The contentSize remains the same no matter the node is scaled or rotated.
     * All nodes has a size. Layer and Scene has the same size of the screen.
     *
     * @param contentSize   The untransformed size of the node.
     */
    virtual void setContentSize(const CCSize& contentSize);
    /**
     * Returns the untransformed size of the node.
     *
     * @see setContentSize(const CCSize&)
     *
     * @return The untransformed size of the node.
     */
    virtual const CCSize& getContentSize() const;


    /**
     * Sets whether the node is visible
     *
     * The default value is true, a node is default to visible
     *
     * @param visible   true if the node is visible, false if the node is hidden.
     */
    virtual void setVisible(bool visible);
    /**
     * Determines if the node is visible
     *
     * @see setVisible(bool)
     *
     * @return true if the node is visible, false if the node is hidden.
     */
    virtual bool isVisible();

    
    /** 
     * Sets the rotation (angle) of the node in degrees. 
     * 
     * 0 is the default rotation angle. 
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     * 
     * @param fRotation     The roration of the node in degrees.
     */
    virtual void setRotation(float fRotation);
    /**
     * Returns the rotation of the node in degrees.
     *
     * @see setRotation(float)
     *
     * @return The rotation of the node in degrees.
     */
    virtual float getRotation();

    
    /** 
     * Sets the X rotation (angle) of the node in degrees which performs a horizontal rotational skew.
     * 
     * 0 is the default rotation angle. 
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     * 
     * @param fRotationX    The X rotation in degrees which performs a horizontal rotational skew.
     */
    virtual void setRotationX(float fRotaionX);
    /**
     * Gets the X rotation (angle) of the node in degrees which performs a horizontal rotation skew.
     *
     * @see setRotationX(float)
     *
     * @return The X rotation in degrees.
     */
    virtual float getRotationX();

    
    /** 
     * Sets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     * 
     * 0 is the default rotation angle. 
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param fRotationY    The Y rotation in degrees.
     */
    virtual void setRotationY(float fRotationY);
    /**
     * Gets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     *
     * @see setRotationY(float)
     *
     * @return The Y rotation in degrees.
     */
    virtual float getRotationY();

	/**
     * Whether or not the transform property changed. if true, the bone will update the transform.
     */
    virtual inline void setTransformDirty(bool dirty) { m_bTransformDirty = dirty; }
    virtual inline bool isTransformDirty() { return m_bTransformDirty; }

	/// @}  end of Setters & Getters for Graphic Peroperties
	 
protected:

	float m_fRotationX;                 ///< rotation angle on x-axis
	float m_fRotationY;                 ///< rotation angle on y-axis

	float m_fScaleX;                    ///< scaling factor on x-axis
	float m_fScaleY;                    ///< scaling factor on y-axis

	float m_fVertexZ;                   ///< OpenGL real Z vertex

	CCPoint m_obPosition;               ///< position of the node

	float m_fSkewX;                     ///< skew angle on x-axis
	float m_fSkewY;                     ///< skew angle on y-axis

	CCPoint m_obAnchorPointInPoints;    ///< anchor point in points
	CCPoint m_obAnchorPoint;            ///< anchor point normalized (NOT in points)

	CCSize m_obContentSize;             ///< untransformed size of the node


	//CCAffineTransform m_sAdditionalTransform; ///< transform
	CCAffineTransform m_sTransform;     ///< transform
	CCAffineTransform m_sInverse;       ///< transform

	int m_nZOrder;                      ///< z-order value that affects the draw order

	bool m_bRunning;                    ///< is running

	bool m_bTransformDirty;             ///< transform dirty flag
	bool m_bInverseDirty;               ///< transform dirty flag
	//bool m_bAdditionalTransformDirty;   ///< The flag to check whether the additional transform is dirty
	bool m_bVisible;                    ///< is this node visible
};

} // namespace

#endif // _CC_NODE_H_