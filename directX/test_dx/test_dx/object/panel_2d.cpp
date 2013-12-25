#include "panel_2d.h"


bool Panel2D::init()
{
	ge::GE_VERTEX_DECL vertex_decl_;
	vertex_decl_.init(DEF_FVF_FORMAT);
	set_vertex_decl(&vertex_decl_);
	
	ge::GE_VERTEX vertex_buff[4];
	for (int i=0; i<4; ++i) vertex_buff[i].set_decl(&vertex_decl_);
	
	vertex_buff[0].set_position(D3DXVECTOR3(-5.0f, -5.0f, 0.f));
	vertex_buff[0].set_texcoords(D3DXVECTOR2(0.f, 0.f));
	vertex_buff[0].set_color(RGBA(0xff, 0xff, 0xff, 0xff));
	
	vertex_buff[1].set_position(D3DXVECTOR3(-5.0f, 5.0f, 0.f));
	vertex_buff[1].set_texcoords(D3DXVECTOR2(0.f, 1.f));
	vertex_buff[1].set_color(RGBA(0xff, 0xff, 0xff, 0xff));

	vertex_buff[2].set_position(D3DXVECTOR3(5.0f, 5.0f, 0.f));
	vertex_buff[2].set_texcoords(D3DXVECTOR2(1.f, 1.f));
	vertex_buff[2].set_color(RGBA(0xff, 0xff, 0xff, 0xff));

	vertex_buff[3].set_position(D3DXVECTOR3(5.0f, -5.0f, 0.f));
	vertex_buff[3].set_texcoords(D3DXVECTOR2(1.f, 0.f));
	vertex_buff[3].set_color(RGBA(0xff, 0xff, 0xff, 0xff));
	set_vertices(vertex_buff, 4);

	WORD index_buff[6];
	index_buff[0] = 0; index_buff[1] = 1; index_buff[2] = 3;
	index_buff[3] = 3; index_buff[4] = 1; index_buff[5] = 2;
	set_indices(index_buff, 6);

	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	HRESULT h_res = S_OK;
	LPDIRECT3DTEXTURE9 img_texture_;
	h_res = D3DXCreateTextureFromFile(p_device, "texture\\spineboy.png", &img_texture_);
	h_res = D3DXCreateTexture(p_device, 250, 250, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &p_texture_);

	D3DSURFACE_DESC surface_desc;
	img_texture_->GetLevelDesc(0, &surface_desc);

	D3DLOCKED_RECT d3d_rect;
	D3DLOCKED_RECT d3d_rect2;
	ge::GE_IRECT rect(0, 0, 100, 100);
	img_texture_->LockRect(0, &d3d_rect, &rect, D3DLOCK_READONLY);
	p_texture_->LockRect(0, &d3d_rect2, &rect, 0);
	char* p_src = (char*)d3d_rect.pBits;
	char* p_dst = (char*)d3d_rect2.pBits;
	int src_pitch = d3d_rect.Pitch;
	int dst_pitch = d3d_rect2.Pitch;
	for (int i=0; i<100; i++)
	{
		memcpy(p_dst, p_src, 400);
		p_src += src_pitch;
		p_dst += dst_pitch;
	}
	p_texture_->UnlockRect(0);
	img_texture_->UnlockRect(0);



	p_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	p_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	p_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	if (SUCCEEDED(h_res))
		p_device->SetTexture(0, p_texture_);

	return SUCCEEDED(h_res);
}

void Panel2D::update( time_t time_elapsed )
{
}

void Panel2D::render( time_t time_elapsed )
{
	ge::GEOModel::render(time_elapsed);
}