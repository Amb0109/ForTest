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
public:
	GEObject();
	virtual ~GEObject();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

	virtual void on_update(time_t time_elapsed);
	virtual void on_render(time_t time_elapsed);

	virtual D3DXMATRIX& get_world_matrix() { return world_matrix_; }

protected:
	virtual void _calc_world_matrix();

protected:
	GEObjectType	type_;
	GETransform		transform_;

	D3DXMATRIX		trans_matrix_;
	D3DXMATRIX		rotatex_matrix_;
	D3DXMATRIX		rotatey_matrix_;
	D3DXMATRIX		rotatez_matrix_;
	D3DXMATRIX		scale_matrix_;
	D3DXMATRIX		world_matrix_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_H_