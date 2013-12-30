#ifndef _GAME_ENGINE_OBJECT_SPINE_H_
#define _GAME_ENGINE_OBJECT_SPINE_H_

#include "../common/ge_include.h"
#include "ge_object.h"
#include "ge_object_def.h"
#include "geo_primitive.h"
#include "spine/spine.h"
#include "spine/extension.h"

namespace ge
{

class GE_API GEAtlasPageManager
{
public:
	static GEAtlasPageManager* get_instence();

public:
	bool create_texture(spAtlasPage*, const char*);
	void dispose_texture(spAtlasPage*);
};

class GE_API GE_VERTEX_DECL;
class GE_API GEOSpine : public GEObject
{
protected:
	typedef GE_VERTEX_DECL::D3D_VERTEX_DECL _D3D_VERTEX_DECL;

public:
	GEOSpine();
	virtual ~GEOSpine();

protected:
	bool _init_mesh();
	void _do_render();

	bool _init_bone_mesh();
	void _do_bone_render();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

private:
	spAtlas*				p_atlas_;
	spSkeleton*				p_skeleton_;
	spSkeletonJson*			p_skeleton_json_;
	spSkeletonData*			p_skeleton_data_;
	spAnimation*			p_animation_;
	spAnimationState*		p_animation_state_;
	spAnimationStateData*	p_animation_state_data_;

	GEOPrimitive			mesh_;
	GE_VERTEX_DECL			vertex_decl_;

	bool					draw_bone_mesh_;
	GEOPrimitive			bone_mesh_;
	GE_VERTEX_DECL			bone_vertex_decl_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_SPINE_H_