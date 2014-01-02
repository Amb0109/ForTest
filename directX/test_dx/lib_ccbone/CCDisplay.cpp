#include "CCDisplay.h"
#include "CCSkin.h"
#include "CCArmatureData.h"

bool CCDisplay::init( CCDisplayData* data, CCBone* bone )
{
	if (m_data == NULL) return false;
	if (m_bone == NULL) return false;

	m_data = data;
	m_bone = bone;

	CCBaseData* skin_data = m_data->get_skin_data();
	if (skin_data == NULL) return false;

	m_skin = new CCSkin();
	m_skin->init(skin_data, bone);

	return true;
}
