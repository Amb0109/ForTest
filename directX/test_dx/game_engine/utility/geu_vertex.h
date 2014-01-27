#ifndef _GAME_ENGINE_UTILITY_VERTEX_H_
#define _GAME_ENGINE_UTILITY_VERTEX_H_

#include "../common/ge_include.h"

#define VERTEX_ELEMENT_MAX_CNT 16

namespace ge
{

class GE_API GEVertexDecl
{
public:
	typedef IDirect3DVertexDeclaration9 D3D_VERTEX_DECL;

public:
	GEVertexDecl();
	virtual ~GEVertexDecl();

public:
	bool				init(DWORD fvf);
	void				release();

	int					get_vertex_size() { return vertex_size_; }
	DWORD				get_vertex_fvf() { return vertex_fvf_; }
	D3D_VERTEX_DECL*	get_d3d_vertex_decl() { return p_vertex_decl_; }
	LPD3DVERTEXELEMENT9	get_vertex_element() { return vertex_element_; }

protected:
	void				_calc_vertex_element_array(int& array_pos, int& mem_size);
	bool				_add_vertex_element(DWORD fvf_type, int& array_pos, int& mem_offset);

private:
	D3DVERTEXELEMENT9	vertex_element_[VERTEX_ELEMENT_MAX_CNT];
	D3D_VERTEX_DECL*	p_vertex_decl_;
	int					vertex_size_;

	DWORD				vertex_fvf_;
};

class GE_API GE_VERTEX
{
public:
	GE_VERTEX();
	virtual ~GE_VERTEX();

public:
	bool			set_decl(GEVertexDecl* decl);

	void			set_position(D3DXVECTOR3 position)	{ position_ = position; }
	void			set_normal(D3DXVECTOR3 normal)		{ normal_ = normal; }
	void			set_texcoords(D3DXVECTOR2 texcoords){ texcoords_ = texcoords; }
	void			set_color(D3DCOLOR color)			{ color_ = color; }

	bool			pack(void* mem_buff, int size);

protected:
	bool			_append_data(void* mem_buff, int& mem_offset, void* p_data, int data_size);

private:
	D3DXVECTOR3			position_;
	D3DXVECTOR3			normal_;
	D3DXVECTOR2			texcoords_;
	D3DCOLOR			color_;

	DWORD				vertex_fvf_;
	int					vertex_size_;
};


}

#endif // _GAME_ENGINE_UTILITY_VERTEX_H_