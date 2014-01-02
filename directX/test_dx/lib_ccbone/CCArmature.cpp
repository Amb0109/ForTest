#include "CCArmature.h"
#include "CCArmatureData.h"
#include "CCBone.h"
#include <algorithm>

CCArmature::CCArmature()
{

}

CCArmature::~CCArmature()
{

}

bool CCArmature::init( CCArmatureData* armature_data )
{
	if(armature_data == NULL) return false;
	m_data = armature_data;

	for (int i=0; ; ++i)
	{
		CCBoneData* bone_data = m_data->get_bone_data(i);
		if (bone_data == NULL) break;

		create_bone(bone_data->m_name.c_str());
	}
	return true;
}

CCBone* CCArmature::create_bone( const char* bone_name )
{
	if (m_data == NULL) return NULL;
	if (bone_name == NULL) return NULL;

	CC_BONE_MAP::iterator exist_bone = m_bones.find(bone_name);
	if(exist_bone != m_bones.end()) return exist_bone->second;

	CCBoneData* bone_data = m_data->get_bone_data_by_name(bone_name);
	if (bone_data == NULL) return NULL;

	CCBone* bone = new CCBone();
	if (bone == NULL) return NULL;
	bone->init(bone_data);

	if (bone_data->m_parent.length() == 0)
	{ // root
		m_root_bones.push_back(bone);
	}
	else
	{
		CCBone* parent_bone = create_bone(bone_data->m_parent.c_str());
		if (parent_bone == NULL)
		{
			delete bone;
			bone = NULL;
			return NULL;
		}

		parent_bone->add_child(bone);
	}

	m_bones[bone_data->m_name] = bone;
	return bone;
}

void CCArmature::update( float delta )
{
	//m_animation->upate();

	FOR_EACH (CC_BONE_MAP, m_bones, it)
	{
		if (CCBone *bone = dynamic_cast<CCBone *>(it->second))
		{
			bone->update(delta);
		}
	}
}

void CCArmature::render( float delta )
{
	transform();
	sort_childs();
	draw();
}

void CCArmature::transform()
{

}

void CCArmature::sort_childs()
{
	if (!m_need_reorder) return;
	std::sort(m_root_bones.begin(), m_root_bones.end(),
		CCArmature::_bone_short_cmp);
	m_need_reorder = false;
}

void CCArmature::draw()
{
	FOR_EACH (CC_BONE_MAP, m_bones, it)
	{
		if (CCBone *bone = dynamic_cast<CCBone *>(it->second))
		{
			//CCSkin* skin = bone->get_skin();
			//skin->update_transform();

		}
	}
}

bool CCArmature::_bone_short_cmp( const CCBone* a, const CCBone* b )
{
	return a->get_draw_order() < b->get_draw_order();
}

