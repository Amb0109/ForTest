#ifndef _CC_DISPLAY_H_
#define _CC_DISPLAY_H_

class CCDisplayData;
class CCBone;
class CCSkin;

class CCDisplay
{
public:
	bool init(CCDisplayData* data, CCBone* bone);

private:
	CCDisplayData*	m_data;
	CCBone*			m_bone;

	CCSkin*			m_skin;
};

#endif // _CC_DISPLAY_H_