#include "geo_light.h"
#include "../common/ge_engine.h"
#include "../render/ge_render.h"

namespace ge
{

GEOLight::GEOLight()
{

}

GEOLight::~GEOLight()
{

}

bool GEOLight::init()
{
	D3DXCOLOR WHITE = 0xffffffff;
	D3DXCOLOR BLACK = 0x000000ff;

	memset(&light_, 0, sizeof(light_));
	light_.Type = D3DLIGHT_POINT;
	light_.Position = D3DXVECTOR3(5.f, 5.f, -5.f);
	light_.Direction = D3DXVECTOR3(-0.57f, -0.57f, 0.57f);
	light_.Ambient = WHITE * 0.4f;
	light_.Diffuse = WHITE;
	light_.Specular = WHITE * 0.8f;
	light_.Attenuation0 = 1.0f;
	light_.Attenuation1 = 0.0f;
	light_.Attenuation2 = 0.0f;
	light_.Range = 100.f;

	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;

	p_d3d_device->SetLight(0, &light_);
	p_d3d_device->LightEnable(0, true);
	return true;
}

void GEOLight::destory()
{

}

void GEOLight::update( time_t time_elapsed )
{
	
}

void GEOLight::render( time_t time_elapsed )
{


}


}