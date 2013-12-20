#include "ger_effect.h"
#include "ge_render.h"
#include "../common/ge_engine.h"
#include "../common/ge_app.h"

namespace ge
{
GEREffect::GEREffect()
:p_fx_(NULL),
p_err_msg_(NULL)
{

}

GEREffect::~GEREffect()
{

}

bool GEREffect::init( const char* file_path )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;

	SAFE_RELEASE(p_fx_);
	SAFE_RELEASE(p_err_msg_);
	if (file_path == NULL) return false;

	file_path_ = file_path;
	HRESULT h_res = D3DXCreateEffectFromFile(
		p_d3d_device,
		file_path,
		NULL, // Defines
		NULL, // Include
#ifdef _DEBUG
		D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
#else
		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
#endif
		NULL, // Pool
		&p_fx_,
		&p_err_msg_);

	return SUCCEEDED(h_res);
}

void GEREffect::release()
{
	file_path_.clear();

	SAFE_RELEASE(p_fx_);
	SAFE_RELEASE(p_err_msg_);
}

const char* GEREffect::get_err_msg()
{
	if (p_err_msg_ == NULL) return NULL;
	return (char*)p_err_msg_->GetBufferPointer();
}

LPD3DXEFFECT GEREffect::get_fx()
{
	return p_fx_;
}


}

