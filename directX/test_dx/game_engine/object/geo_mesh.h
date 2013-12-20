#ifndef _GAME_ENGINE_OBJECT_MESH_H_
#define _GAME_ENGINE_OBJECT_MESH_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"
#include "../render/ger_effect.h"

namespace ge
{

class GE_API GEOMesh : public GEObject
{
public:
	GEOMesh();
	virtual ~GEOMesh();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

public:
	virtual bool init_effect();
	virtual bool update_effect();

protected:
	LPD3DXMESH		p_mesh_;
	GEREffect		effect_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_MESH_H_