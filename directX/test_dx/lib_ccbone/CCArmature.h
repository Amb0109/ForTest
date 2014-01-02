#ifndef _CC_ARMATURE_H_
#define _CC_ARMATURE_H_

#include "CCAnimationData.h"

class CCBone;
class CCArmature
{
public:
	typedef std::map<std::string, CCBone*> CC_BONE_MAP;

public:
	CCArmature();
	virtual ~CCArmature();

public:
	bool		init(CCArmatureData* armature_data);
	CCBone*		create_bone(const char* bone_name);

	//bool		set_parent_bone(CCBone* parent_bone);
	//CCBone*		get_parent_bone();

	void		update(float delta);
	void		update_transform();

	void		render(float delta);
	void		transform();
	void		sort_childs();
	void		draw();

protected:
	static bool	_bone_short_cmp(const CCBone* a, const CCBone* b);

private:
	CCArmatureData*						m_data;
	std::vector<CCBone*>				m_root_bones;
	CC_BONE_MAP							m_bones;

	bool								m_need_reorder;
};

#endif // _CC_ARMATURE_H_