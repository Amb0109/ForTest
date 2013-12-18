#ifndef _GAME_ENGINE_UTILITY_VERTEX_H_
#define _GAME_ENGINE_UTILITY_VERTEX_H_

#include "../common/ge_include.h"

#define VERTEX_BUFF_MAX_SIZE 1024
#define VERTEX_ELEMENT_MAX_CNT 16

namespace ge
{

struct GE_VERTEX_STRUCT
{
	float x, y, z;
	D3DCOLOR color;
};

class GE_API GE_VERTEX
{
public:
	typedef LPDIRECT3DVERTEXDECLARATION9 P_VERTEX_DECL;

public:
	GE_VERTEX();
	virtual ~GE_VERTEX(){}

public:
	bool			set_fvf(DWORD fvf);
	DWORD			get_fvf() { return fvf_; }
	P_VERTEX_DECL	get_decl() { return p_vertex_decl_; }
	int				get_size() { return sizeof(test_body); }// return vertex_size_; }

	void			set_position(D3DXVECTOR3 position)	{ position_ = position; }
	void			set_normal(D3DXVECTOR3 normal)		{ normal_ = normal; }
	void			set_texcoords(D3DXVECTOR2 texcoords){ texcoords_ = texcoords; }
	void			set_color(D3DCOLOR color)			{ color_ = color; }

	const void*		pack();

protected:
	void			_calc_vertex_element_array(int& array_pos, int& mem_size);
	bool			_add_vertex_element(DWORD fvf_type, int& array_pos, int& mem_offset);

	bool			_append_data(int& mem_offset, void* p_data, int data_size);

private:
	D3DXVECTOR3			position_;
	D3DXVECTOR3			normal_;
	D3DXVECTOR2			texcoords_;
	D3DCOLOR			color_;
	DWORD				fvf_;

	int					vertex_size_;
	char				vertex_buff_[VERTEX_BUFF_MAX_SIZE];
	D3DVERTEXELEMENT9	vertex_element_[VERTEX_ELEMENT_MAX_CNT];
	P_VERTEX_DECL		p_vertex_decl_;

	GE_VERTEX_STRUCT	test_body;
};


}

#endif // _GAME_ENGINE_UTILITY_VERTEX_H_