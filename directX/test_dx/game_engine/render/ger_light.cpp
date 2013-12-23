#include "ger_light.h"
#include "../common/ge_engine.h"
#include "../render/ge_render.h"

namespace ge
{

GE_LIGHT::GE_LIGHT()
: light_index_(-1)
{
	ZeroMemory(&light_, sizeof(light_));
}

GE_LIGHT::GE_LIGHT( D3DLIGHTTYPE type )
: light_index_(-1)
{
	ZeroMemory(&light_, sizeof(light_));
	light_.Type = type;
}

GE_LIGHT::~GE_LIGHT()
{
	disable();
}

int GE_LIGHT::_get_light_index()
{
	static int _g_light_auto_index = -1;
	return ++_g_light_auto_index;
}

bool GE_LIGHT::enable()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;

	HRESULT h_res = S_OK;
	if (light_index_ == -1)
	{
		light_index_ = GE_LIGHT::_get_light_index();
		h_res = p_d3d_device->SetLight(light_index_, &light_);
		if (FAILED(h_res)) return false;
	}

	h_res = p_d3d_device->LightEnable(light_index_, true);
	return SUCCEEDED(h_res);
}

bool GE_LIGHT::disable()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;

	if (light_index_ == -1) return false;
	
	HRESULT h_res = S_OK;
	h_res = p_d3d_device->SetLight(light_index_, &light_);
	return SUCCEEDED(h_res);
}

void GE_LIGHT::set_type( D3DLIGHTTYPE type )
{
	light_.Type = type;
}

void GE_LIGHT::set_position( float pos_x, float pos_y, float pos_z )
{
	light_.Position = D3DXVECTOR3(pos_x, pos_y, pos_z);
}

void GE_LIGHT::set_direction( float dir_x, float dir_y, float dir_z )
{
	light_.Direction = D3DXVECTOR3(dir_x, dir_y, dir_z);
}

void GE_LIGHT::set_color( D3DXCOLOR ambient_color, D3DXCOLOR diffuse_color, D3DXCOLOR specular_color )
{
	light_.Ambient = ambient_color;
	light_.Diffuse = diffuse_color;
	light_.Specular = specular_color;
}

void GE_LIGHT::set_range( float range )
{
	light_.Range = range;
}

void GE_LIGHT::set_attenuation( float atten0, float atten1, float atten2 )
{
	light_.Attenuation0 = atten0;
	light_.Attenuation1 = atten1;
	light_.Attenuation2 = atten2;
}

void GE_LIGHT::set_spotlight_info( float falloff, float theta, float phi )
{
	light_.Falloff = falloff;
	light_.Theta = theta;
	light_.Phi = phi;
}

}