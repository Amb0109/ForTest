#include "CCBone.h"
#include "CCData.h"
#include "CCDisplay.h"
#include "CCArmature.h"
#include "CCArmatureData.h"
#include <algorithm>

CCBone::CCBone()
{

}

CCBone::~CCBone()
{

}

bool CCBone::init( CCBoneData* bone_data )
{
	if (bone_data == NULL) return false;
	m_data = bone_data;
	m_draw_order = bone_data->zOrder;

	for (int i=0; ; i++)
	{
		CCDisplayData* display_data = bone_data->get_display_data(i);
		if (display_data == NULL) break;
		switch (display_data->m_displayType)
		{
		case CC_DISPLAY_SPRITE:
			{
				CCDisplay* display = new CCDisplay();
				if (display != NULL)
				{
					display->init(display_data, this);
					m_display_lst.push_back(display);
				}
			}
			break;
		default:
			break;
		}
	}

	return true;
}

bool CCBone::set_armature( CCArmature* armature )
{
	if(armature == NULL) return false;
	m_armature = armature;
	//m_armature_parent_bone = armature->get_parent_bone();
	return true;
}

void CCBone::set_parent( CCBone* parent_bone )
{
	m_parent = parent_bone;
}

bool CCBone::add_child( CCBone* child_bone )
{
	if (child_bone == NULL) return false;

	if(m_childs.end() == std::find(m_childs.begin(), m_childs.end(), child_bone))
	{
		m_childs.push_back(child_bone);
	}

	child_bone->set_parent(this);
	return true;
}

void CCBone::apply_parent_transform( CCBone* parent )
{
	float x = m_world_info->x;
	float y = m_world_info->y;
	m_world_info->x = x * parent->m_world_transform.a + y * parent->m_world_transform.c + parent->m_world_info->x;
	m_world_info->y = x * parent->m_world_transform.b + y * parent->m_world_transform.d + parent->m_world_info->y;
	m_world_info->scaleX = m_world_info->scaleX * parent->m_world_info->scaleX;
	m_world_info->scaleY = m_world_info->scaleY * parent->m_world_info->scaleY;
	m_world_info->skewX = m_world_info->skewX + parent->m_world_info->skewX;
	m_world_info->skewY = m_world_info->skewY + parent->m_world_info->skewY;
}

void CCBone::update( float delta )
{
	if (m_parent != NULL)
	{
		apply_parent_transform(m_parent);
	}

	FOR_EACH (std::vector<CCBone*>, m_childs, it)
	{
		((CCBone*)&(*it))->update(delta);
	}
}

int CCBone::get_draw_order() const
{
	return m_draw_order;
}

