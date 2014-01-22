#ifndef _GAME_ENGINE_OBJECT_MESH_H_
#define _GAME_ENGINE_OBJECT_MESH_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"

namespace ge
{

class GE_VERTEX_DECL;
class GE_VERTEX;
class GE_MATERIAL;
class GEREffect;
class GE_API GEOMesh : public GEObject
{
public:
	GEOMesh();
	virtual ~GEOMesh();

public:
	bool create_mesh(GE_VERTEX_DECL* vertex_decl, int vertex_cnt, int face_cnt);
	bool set_vertices(GE_VERTEX* vertex_array, int vertex_cnt);
	bool set_indices(WORD* index_array, int index_cnt);

	bool create_mesh_from_file(const char* file_name);	
	bool save_mesh_to_file(const char* file_name);

	bool create_mesh_box(float width, float height, float depth);
	bool create_mesh_cylinder(float radius1, float radius2, float length, int slices, int stacks);
	bool create_mesh_sphere(float radius, int slices, int stacks);
	bool create_mesh_teapot();
	bool create_mesh_torus(float in_radius, float out_radius, int sides, int rings);

	bool set_material(GE_MATERIAL* p_material);
	bool set_effect(GEREffect* p_effect);

protected:
	bool _get_infos_from_mesh();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t delta);
	virtual void render(time_t delta);

	virtual void on_render(time_t delta);

protected:
	LPD3DXMESH		p_mesh_;
	GEREffect*		p_effect_;
	GE_MATERIAL*	p_material_;

	int				vertex_cnt_;
	int				face_cnt_;
	int				vertex_size_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_MESH_H_