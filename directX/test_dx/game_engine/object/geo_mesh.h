#ifndef _GAME_ENGINE_OBJECT_MESH_H_
#define _GAME_ENGINE_OBJECT_MESH_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"
#include "../render/ger_shader.h"

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

protected:
	ID3DXMesh*		p_mesh_;

	ge::GERShader test_shader;

	float			model_rotation_y;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_MESH_H_