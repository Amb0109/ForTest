#include "CCDisplay.h"
#include "CCSkin.h"
#include "../data/CCArmatureData.h"

namespace CC {

CCDisplay::CCDisplay()
: m_pData(NULL)
, m_pBone(NULL)
, m_pSkin(NULL)
, m_type(CC_DISPLAY_MAX)
{

}

CCDisplay::~CCDisplay()
{
	release();
}

void CCDisplay::release()
{
	m_pData = NULL;
	m_pBone = NULL;

	if (m_pSkin)
	{
		m_pSkin->release();
		delete m_pSkin;
		m_pSkin = NULL;
	}
}

bool CCDisplay::init( CCDisplayData* data, CCBone* bone )
{
	if (data == NULL) return false;
	if (bone == NULL) return false;

	m_pData = data;
	m_pBone = bone;
	m_type = (CCDisplayType)m_pData->m_eDisplayType;

	m_pSkin = new CCSkin();
	m_pSkin->init(m_pData, bone);

	return true;
}

CC::CCDisplayType CCDisplay::get_type()
{
	return m_type;
}

void CCDisplay::update( float delta )
{
	if (m_pSkin != NULL)
	{
		m_pSkin->update(delta);
	}
}

} // namespace