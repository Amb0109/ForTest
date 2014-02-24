#include "ger_effect.h"
#include "ge_render.h"
#include "../common/ge_engine.h"
#include "../common/ge_app.h"
#include "../object/ge_object.h"

namespace ge
{
GEREffect::GEREffect()
:p_fx_(NULL),
p_err_msg_(NULL)
{

}

GEREffect::~GEREffect()
{
	destory();
}

bool GEREffect::create_from_file( const char* file_path )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;

	D3D_RELEASE(p_fx_);
	D3D_RELEASE(p_err_msg_);
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

const char* GEREffect::get_err_msg()
{
	if (p_err_msg_ == NULL) return NULL;
	return (char*)p_err_msg_->GetBufferPointer();
}

bool GEREffect::init()
{
	return true;
}

void GEREffect::destory()
{
	file_path_.clear();

	D3D_RELEASE(p_fx_);
	D3D_RELEASE(p_err_msg_);
}

bool GEREffect::set_technique( const char* name )
{
	if (p_fx_ == NULL) return false;

	HRESULT h_res = p_fx_->SetTechnique(name);
	return SUCCEEDED(h_res);
}

int GEREffect::begin_effect()
{
	if (p_fx_ == NULL) return -1;

	int pass_num = 0;
	HRESULT h_res = p_fx_->Begin((UINT*)&pass_num, 0);
	if (FAILED(h_res)) return -1;
	return pass_num;
}

bool GEREffect::end_effect()
{
	if (p_fx_ == NULL) return false;
	
	HRESULT h_res = p_fx_->End();
	return SUCCEEDED(h_res);
}

bool GEREffect::begin_pass( int pass_id )
{
	if (p_fx_ == NULL) return false;

	HRESULT h_res = p_fx_->BeginPass((UINT)pass_id);
	return SUCCEEDED(h_res);
}

bool GEREffect::end_pass()
{
	if (p_fx_ == NULL) return false;

	HRESULT h_res = p_fx_->EndPass();
	return SUCCEEDED(h_res);
}

}

