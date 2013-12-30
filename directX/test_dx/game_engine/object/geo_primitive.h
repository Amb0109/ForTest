#ifndef _GAME_ENGINE_OBJECT_PRIMITIVE_H_
#define _GAME_ENGINE_OBJECT_PRIMITIVE_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"
#include "../utility/geu_vertex.h"

#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_TEX1)

namespace ge
{

class GE_API GE_VERTEX;
class GE_API GEREffect;
class GE_API GEOPrimitive : public GEObject
{
protected:
	typedef GE_VERTEX_DECL::D3D_VERTEX_DECL _D3D_VERTEX_DECL;

public:
	GEOPrimitive();
	virtual ~GEOPrimitive();

public:
	virtual bool set_vertex_decl(GE_VERTEX_DECL* vertex_decl);
	virtual void release_vertex_decl();

	virtual bool create_vetrix_buff(int vertex_cnt);
	virtual bool set_vertices(GE_VERTEX* vertex_array, int offset, int vertex_cnt);
	virtual void release_vetrix_buff();
	
	virtual bool create_index_buff(int index_cnt);
	virtual bool set_indices(WORD* index_array, int offset, int index_cnt);
	virtual void release_index_buff();

	virtual void set_primitive_draw(int start, int cnt);

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

protected:
	LPDIRECT3DVERTEXBUFFER9	d3d_vertex_buff_;
	LPDIRECT3DINDEXBUFFER9	d3d_index_buff_;
	int						vertex_cnt_;
	int						index_cnt_;

	_D3D_VERTEX_DECL*		p_d3d_decl_;
	int						vertex_size_;

	int						draw_primitive_start_;
	int						draw_primitive_cnt_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_PRIMITIVE_H_