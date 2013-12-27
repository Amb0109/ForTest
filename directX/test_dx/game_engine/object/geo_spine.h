#ifndef _GAME_ENGINE_OBJECT_SPINE_H_
#define _GAME_ENGINE_OBJECT_SPINE_H_

#include "../common/ge_include.h"
#include "ge_object.h"
#include "ge_object_def.h"
#include "geo_model.h"
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

class GE_API GEOSpine : public GEObject
{
public:
	GEOSpine();
	virtual ~GEOSpine();

protected:
	bool _init_draw_panel();

	bool _load_region_texture(const spAtlasRegion* atlas_region);
	bool _transform_region_texture(const spRegionAttachment* region_attachment, const spBone* bone);

	bool _init_bone_mesh();
	bool _render_bone();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

private:
	spAtlas*			p_atlas_;
	spSkeleton*			p_skeleton_;
	spSkeletonJson*		p_json_;
	spSkeletonData*		p_skeleton_data_;
	spAnimation*		p_animation_;
	spAnimationState*	p_animation_state_;
	spAnimationStateData* p_animation_state_data_;

	GEOModel			mesh_;

	GEOModel			bone_mesh_;
	bool				draw_bone_mesh_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_SPINE_H_