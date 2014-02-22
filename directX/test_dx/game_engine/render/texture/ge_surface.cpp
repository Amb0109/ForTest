#include "ge_surface.h"
#include "../../common/ge_engine.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GESurface);

GESurface::GESurface()
: d3d_surface_(NULL)
{

}

GESurface::~GESurface()
{
	destory();
}

bool GESurface::init( int width, int height, D3DFORMAT format /*= D3DFMT_UNKNOWN*/)
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	destory();

	HRESULT h_res = S_OK;
	h_res = p_d3d_device->CreateOffscreenPlainSurface(
		width, height,
		format,
		D3DPOOL_DEFAULT,
		&d3d_surface_,
		NULL);

	assert(SUCCEEDED(h_res));
	if (FAILED(h_res)) return false;
	if (d3d_surface_ == NULL) return false;

	d3d_surface_->GetDesc(&surface_desc_);
	return true;
}

void GESurface::destory()
{
	D3D_RELEASE(d3d_surface_);
}

bool GESurface::begin_dc( HDC& h_dc )
{
	if (d3d_surface_ == NULL) return false;
	return SUCCEEDED(d3d_surface_->GetDC(&h_dc));
}

bool GESurface::end_dc( HDC& h_dc )
{
	if (d3d_surface_ == NULL) return false;
	HRESULT h_res = d3d_surface_->ReleaseDC(h_dc);
	if (FAILED(h_res)) return false;
	h_dc = NULL;
	return true;
}

LPDIRECT3DSURFACE9 GESurface::get_d3d_surface()
{
	return d3d_surface_;
}

void GESurface::get_size( int& width, int& height )
{
	width = surface_desc_.Width;
	height = surface_desc_.Height;
}

D3DFORMAT GESurface::get_format()
{
	return surface_desc_.Format;
}

}