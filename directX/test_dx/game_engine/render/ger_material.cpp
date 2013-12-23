#include "ger_material.h"
#include "../common/ge_engine.h"

namespace ge
{

void GE_MATERIAL::set_default_material( GE_MATERIAL& default_material )
{
	GE_MATERIAL& g_default_material = get_default_material();
	g_default_material = default_material;
}

GE_MATERIAL& GE_MATERIAL::get_default_material()
{
	static GE_MATERIAL _g_default_material;
	return _g_default_material;
}

bool GE_MATERIAL::use_default_material()
{
	LPDIRECT3DDEVICE9 p_device = GEEngine::get_device();
	if (p_device == NULL) return false;
	p_device->SetMaterial(get_default_material().get_d3d_material());
	return true;
}

GE_MATERIAL::GE_MATERIAL()
{
	ZeroMemory(&d3d_material_, sizeof(d3d_material_));
}

GE_MATERIAL::~GE_MATERIAL()
{

}

void GE_MATERIAL::set_ambient_color( D3DXCOLOR ambient_color )
{
	d3d_material_.Ambient = ambient_color;
}

void GE_MATERIAL::set_diffuse_color( D3DXCOLOR diffuse_color )
{
	d3d_material_.Diffuse = diffuse_color;
}

void GE_MATERIAL::set_specular_color( D3DXCOLOR specular_color )
{
	d3d_material_.Specular = specular_color;
}

void GE_MATERIAL::set_emissive_color( D3DXCOLOR emissive_color )
{
	d3d_material_.Emissive = emissive_color;
}

void GE_MATERIAL::set_color( D3DXCOLOR ambient_color, D3DXCOLOR diffuse_color, D3DXCOLOR specular_color, D3DXCOLOR emissive_color )
{
	d3d_material_.Ambient = ambient_color;
	d3d_material_.Diffuse = diffuse_color;
	d3d_material_.Specular = specular_color;
	d3d_material_.Emissive = emissive_color;
}

void GE_MATERIAL::set_power( float power )
{
	d3d_material_.Power = power;
}

}

