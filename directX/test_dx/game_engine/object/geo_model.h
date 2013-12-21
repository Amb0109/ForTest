#ifndef _GAME_ENGINE_OBJECT_MODEL_H_
#define _GAME_ENGINE_OBJECT_MODEL_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"

namespace ge
{

class GE_API GE_VERTEX;
class GE_API GEREffect;
class GE_API GEOModel : public GEObject
{
protected:
	typedef GE_VERTEX_DECL::D3D_VERTEX_DECL _D3D_VERTEX_DECL;

public:
	GEOModel();
	virtual ~GEOModel();

public:
	virtual bool set_vertex_decl(GE_VERTEX_DECL* vertex_decl);
	virtual void release_vertex_decl();

	virtual bool set_vertices(GE_VERTEX* vertex_array, int vertex_cnt);
	virtual bool set_indices(WORD* index_array, int index_cnt);

protected:
	virtual bool _create_vetrix_buff(int vertex_cnt);
	virtual bool _create_index_buff(int index_cnt);
	virtual void _release_vetrix_buff();
	virtual void _release_index_buff();

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
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_MODEL_H_