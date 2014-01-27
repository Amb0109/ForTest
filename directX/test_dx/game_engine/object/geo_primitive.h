#ifndef _GAME_ENGINE_OBJECT_PRIMITIVE_H_
#define _GAME_ENGINE_OBJECT_PRIMITIVE_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"
#include "../utility/geu_vertex.h"

#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_TEX1)

namespace ge
{

class GE_VERTEX;
class GEREffect;
class GE_API GEOPrimitive : public GEObject
{
protected:
	typedef GEVertexDecl::D3D_VERTEX_DECL _D3D_VERTEX_DECL;

	DLL_MANAGE_CLASS(GEOPrimitive);
public:

	GEOPrimitive();
	virtual ~GEOPrimitive();

public:
	virtual bool set_vertex_decl(GEVertexDecl* vertex_decl);
	virtual GEVertexDecl* get_vertex_decl();
	virtual void release_vertex_decl();

	virtual bool create_vetrix_buff(int vertex_cnt);
	virtual int  get_vertex_buff_size();
	virtual bool set_vertices(GE_VERTEX* vertex_array, int offset, int vertex_cnt);
	virtual void release_vetrix_buff();
	
	virtual bool create_index_buff(int index_cnt);
	virtual int  get_index_buff_size();
	virtual bool set_indices(WORD* index_array, int offset, int index_cnt);
	virtual void release_index_buff();

	virtual bool create_texture(const char* texture_path);
	virtual void release_texture();
	virtual void get_texture_size(int& width, int& height);

	virtual void set_primitive_draw(int start, int cnt);

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t delta);
	virtual void render(time_t delta);

protected:
	LPDIRECT3DVERTEXBUFFER9	d3d_vertex_buff_;
	LPDIRECT3DINDEXBUFFER9	d3d_index_buff_;
	int						vertex_cnt_;
	int						index_cnt_;

	LPDIRECT3DTEXTURE9		d3d_texture_;
	D3DSURFACE_DESC			texture_desc_;

	GEVertexDecl			vertex_decl_;
	int						vertex_size_;
	_D3D_VERTEX_DECL*		p_d3d_decl_;

	int						draw_primitive_start_;
	int						draw_primitive_cnt_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_PRIMITIVE_H_