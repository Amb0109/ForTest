#include "ge_texture.h"
#include "../../common/ge_engine.h"
#include "../../utility/geu_gmath.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GETexture);

GETexture::GETexture()
: d3d_texture_(NULL)
{
}

GETexture::~GETexture()
{
	destory();
}

bool GETexture::init( const char* texture_path )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateTextureFromFileEx(
		p_d3d_device,			// pDevice
		texture_path,			// pSrcFile
		D3DX_DEFAULT,			// Width
		D3DX_DEFAULT,			// Height
		D3DX_FROM_FILE,			// MipLevels
		NULL,					// Usage
		D3DFMT_UNKNOWN,			// Format
		D3DPOOL_MANAGED,		// Pool
		D3DX_DEFAULT,			// Filter
		D3DX_DEFAULT,			// MipFilter
		0xff000000,				// ColorKey
		NULL,					// _Inout_ pSrcInfo
		NULL,					// _Out_ pPalette
		&d3d_texture_);			// _Out_ ppTexture

	assert(SUCCEEDED(h_res));
	if (FAILED(h_res) || d3d_texture_ == NULL) return false;

	texture_key_ = texture_path;
	
	d3d_surface_ = NULL;
	d3d_texture_->GetSurfaceLevel(0, &d3d_surface_);
	d3d_texture_->GetLevelDesc(0, &surface_desc_);
	return true;
}

bool GETexture::init( int width, int height, D3DFORMAT format/* = D3DFMT_UNKNOWN*/ )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateTexture(
		p_d3d_device,
		width,
		height,
		1,
		NULL,
		format,
		D3DPOOL_MANAGED,
		&d3d_texture_);

	assert(SUCCEEDED(h_res));
	if (FAILED(h_res) || d3d_texture_ == NULL) return false;

	texture_key_ = "$$";
	d3d_texture_->GetSurfaceLevel(0, &d3d_surface_);
	d3d_texture_->GetLevelDesc(0, &surface_desc_);
	return true;
}

void GETexture::destory()
{
	GESurface::destory();
	D3D_RELEASE(d3d_texture_);
}

bool GETexture::use_texture()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;
	
	HRESULT h_res = p_d3d_device->SetTexture(0, d3d_texture_);
	assert(SUCCEEDED(h_res));
	return true;
}

bool GETexture::use_null_texture()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	HRESULT h_res = p_d3d_device->SetTexture(0, NULL);
	assert(SUCCEEDED(h_res));
	return true;
}

const char* GETexture::get_key()
{
	return texture_key_.c_str();
}

LPDIRECT3DTEXTURE9 GETexture::get_d3d_texture()
{
	return d3d_texture_;
}

}