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

const char* GEREffect::get_err_msg()
{
	if (p_err_msg_ == NULL) return NULL;
	return (char*)p_err_msg_->GetBufferPointer();
}

bool GEREffect::init()
{
	return true;
}

void GEREffect::render( GEObject* obj, time_t time_elapsed )
{
	if (p_fx_ == NULL) return;

	int pass_num = 0;
	p_fx_->Begin((UINT*)&pass_num, 0);
	for(int i = 0; i < pass_num; ++i)
	{
		p_fx_->BeginPass(i);
		obj->on_render(time_elapsed);
		p_fx_->End();
	}
	p_fx_->End();
}

void GEREffect::destory()
{
	file_path_.clear();

	SAFE_RELEASE(p_fx_);
	SAFE_RELEASE(p_err_msg_);

	h_tech_main_ = NULL;
}

}

