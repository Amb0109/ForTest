#ifndef _GAME_ENGINE_UTILITY_VERTEX_H_
#define _GAME_ENGINE_UTILITY_VERTEX_H_

#include "../common/ge_include.h"

namespace ge
{

typedef LPDIRECT3DVERTEXDECLARATION9 D3D_VERTEX_DECL;

struct GE_API GE_VERTEX_DECL
{
	DWORD				fvf;
	int					size;
	D3D_VERTEX_DECL		decl;

	GE_VERTEX_DECL() :fvf(NULL), size(0), decl(NULL) {}
	bool is_valid()
	{
		if (fvf == NULL)	return false;
		if (size <= 0)		return false;
		if (decl == NULL)	return false;
		return true;
	}
};

class GE_API GEVertexDecl
{
public:
	GEVertexDecl();
	virtual ~GEVertexDecl();

public:
	static GEVertexDecl* get_instance();

public:
	static GE_VERTEX_DECL* get_vertex_decl(DWORD fvf);

protected:
	bool				_init_vertex_decl(GE_VERTEX_DECL* out_decl, DWORD fvf);
	bool				_create_vertex_decl(DWORD fvf);
	GE_VERTEX_DECL*		_get_vertex_decl(DWORD fvf);
	void				_destory_vertex_decl(GE_VERTEX_DECL* out_decl);
	void				_release_vertex_decl(DWORD fvf);

	void				_calc_vertex_element_array(DWORD fvf, int& array_pos, int& mem_size);
	bool				_update_vertex_element(DWORD fvf, DWORD add_fvf, int& array_pos, int& mem_offset);
	bool				_push_vertex_element(
							BYTE type, BYTE usage, BYTE usage_index,
							int& array_pos, int& mem_offset, int elem_size);

private:
	D3DVERTEXELEMENT9	vertex_element_[MAX_FVF_DECL_SIZE];

	typedef std::map<DWORD, GE_VERTEX_DECL*> VERTEX_DECL_MAP;
	VERTEX_DECL_MAP		vertex_decl_map_;
};

class GE_API GE_VERTEX
{
public:
	GE_VERTEX();
	virtual ~GE_VERTEX();

public:
	bool			set_fvf(DWORD fvf);
	bool			set_decl(GE_VERTEX_DECL* decl);
	GE_VERTEX_DECL*	get_decl();

	void			set_position(float x, float y, float z);
	void			set_normal(float x, float y, float z);
	void			set_texcoords(float u, float v);
	void			set_blend(UINT blend);
	void			set_color(D3DCOLOR color);

	bool			pack(void* mem_buff, int size);

protected:
	bool			_append_data(void* mem_buff, int& mem_offset, void* p_data, int data_size);

private:
	D3DXVECTOR3			position_;
	D3DXVECTOR3			normal_;
	D3DXVECTOR2			texcoords_;
	UINT				blend_;
	D3DCOLOR			color_;

	GE_VERTEX_DECL*		decl_;
};


}

#endif // _GAME_ENGINE_UTILITY_VERTEX_H_