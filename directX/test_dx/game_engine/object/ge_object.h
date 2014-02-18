#ifndef _GAME_ENGINE_OBJECT_H_
#define _GAME_ENGINE_OBJECT_H_

#include "../common/ge_include.h"
#include "../common/ge_engine.h"
#include "ge_object_def.h"


namespace ge
{

struct GETransform
{
	float px, py, pz;	// Î»ÖÃ
	float rx, ry, rz;	// Ðý×ª
	float sx, sy, sz;	// Ëõ·Å
};

class GE_API GEObject
{
	DLL_MANAGE_CLASS(GEObject);

public:
	GEObject();
	virtual ~GEObject();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t delta);
	virtual void render(time_t delta);

	virtual void on_update(time_t delta);
	virtual void on_render(time_t delta);

public:
	virtual D3DXMATRIX& get_world_transform();
	virtual void set_transform_dirty(bool is_dirty);

	virtual GETransform& get_transform();
	virtual void set_transform(GETransform& transform);

protected:
	GEObjectType	type_;

	GETransform		transform_;
	D3DXMATRIX		d3d_world_transform_;
	bool			is_transform_dirty_;
};

} // namespace ge



#endif // _GAME_ENGINE_OBJECT_H_