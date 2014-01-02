#ifndef _CC_BONE_H_
#define _CC_BONE_H_

#include <./support/CCAffineTransform.h>
#include <vector>

class CCBaseData;
class CCBoneData;
class CCArmature;
class CCDisplay;

class CCBone
{
public:
	CCBone();
	virtual ~CCBone();

public:
	bool init(CCBoneData* bone_data);
	bool set_armature(CCArmature* armature);
	void set_parent(CCBone* parent_bone);
	bool add_child(CCBone* child_bone);

	int get_draw_order() const;

	void update(float delta);
	void apply_parent_transform(CCBone* parent);


private:
	CCBoneData*				m_data;
	CCBone*					m_parent;
	std::vector<CCBone*>	m_childs;

	CCArmature*				m_armature;
	CCBone*					m_armature_parent_bone;

	int						m_draw_order;

	std::vector<CCDisplay*>	m_display_lst;

public:
	CCBaseData*				m_world_info;
	CCAffineTransform		m_world_transform;
};

#endif // _CC_BONE_H_