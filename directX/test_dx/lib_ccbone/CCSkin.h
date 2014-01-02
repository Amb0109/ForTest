#ifndef _CC_SKIN_H_
#define _CC_SKIN_H_

#include "CCSprite.h"
#include "support/CCAffineTransform.h"

class CCBaseData;
class CCBone;

class CCSkin : public CCSprite
{
public:
	CCSkin();
	virtual ~CCSkin();

public:
	bool init(CCBaseData* data, CCBone* bone);
	void updateTransform();

private:
	CCBaseData*			m_data;

	CCAffineTransform	m_skin_transform;
};

#endif // _CC_SKIN_H_