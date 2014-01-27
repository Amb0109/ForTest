#include "ge_texture.h"
#include "../../common/ge_engine.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GETexture);

GETexture::GETexture()
: d3d_texture_(NULL)
{
	ZeroMemory(&texture_desc_, sizeof(D3DSURFACE_DESC));
}

GETexture::~GETexture()
{
	release_texture();
}

bool GETexture::create_texture( const char* texture_path )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	release_texture();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateTextureFromFile(p_d3d_device, texture_path, &d3d_texture_);
	if (FAILED(h_res) || d3d_texture_ == NULL) return false;

	texture_key_ = texture_path;
	d3d_texture_->GetLevelDesc(0, &texture_desc_);
	return true;
}

void GETexture::release_texture()
{
	SAFE_RELEASE(d3d_texture_);
	ZeroMemory(&texture_desc_, sizeof(D3DSURFACE_DESC));
}

bool GETexture::use_texture()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;
	
	p_d3d_device->SetTexture(0, d3d_texture_);
	return true;
}

bool GETexture::use_null_texture()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	p_d3d_device->SetTexture(0, NULL);
	return true;
}

void GETexture::get_texture_size( int& width, int& height )
{
	width = texture_desc_.Width;
	height = texture_desc_.Height;
}

const char* GETexture::get_key()
{
	return texture_key_.c_str();
}

}