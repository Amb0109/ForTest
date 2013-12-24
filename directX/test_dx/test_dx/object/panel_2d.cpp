#include "panel_2d.h"


bool Panel2D::init()
{
	ge::GE_VERTEX_DECL vertex_decl_;
	vertex_decl_.init(DEF_FVF_FORMAT);
	set_vertex_decl(&vertex_decl_);
	
	ge::GE_VERTEX vertex_buff[4];
	for (int i=0; i<4; ++i) vertex_buff[i].set_decl(&vertex_decl_);
	
	vertex_buff[0].set_position(D3DXVECTOR3(-5.0f, -5.0f, 0.f));
	vertex_buff[0].set_color(RGBA(0xff, 0xff, 0xff, 0xff));
	
	vertex_buff[1].set_position(D3DXVECTOR3(-5.0f, 5.0f, 0.f));
	vertex_buff[1].set_color(RGBA(0xff, 0xff, 0xff, 0xff));

	vertex_buff[2].set_position(D3DXVECTOR3(5.0f, 5.0f, 0.f));
	vertex_buff[2].set_color(RGBA(0xff, 0xff, 0xff, 0xff));

	vertex_buff[3].set_position(D3DXVECTOR3(5.0f, -5.0f, 0.f));
	vertex_buff[3].set_color(RGBA(0xff, 0xff, 0xff, 0xff));
	set_vertices(vertex_buff, 4);

	WORD index_buff[6];
	index_buff[0] = 0; index_buff[1] = 1; index_buff[2] = 3;
	index_buff[3] = 3; index_buff[4] = 1; index_buff[5] = 2;
	set_indices(index_buff, 6);
	return true;
}

void Panel2D::update( time_t time_elapsed )
{
}
