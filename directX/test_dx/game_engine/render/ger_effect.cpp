#include "ger_effect.h"
#include "ge_render.h"
#include "../common/ge_engine.h"
#include "../common/ge_app.h"
#include "../object/ge_object.h"
#include "../render/texture/ge_texture.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GEREffect);

GEREffect::GEREffect()
:p_fx_(NULL),
p_err_msg_(NULL)
{
	GEEngine::get_instance()->register_device_object(this);
}

GEREffect::~GEREffect()
{
	destory();
	GEEngine::get_instance()->unregister_device_object(this);
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

bool GEREffect::set_texture( const char* semantic, GETexture* texture )
{
	if (p_fx_ == NULL) return false;

	D3DXHANDLE h_param = NULL;
	h_param = p_fx_->GetParameterBySemantic(NULL, semantic);
	if (h_param == NULL) return false;
	
	HRESULT h_res = S_OK;
	if (texture == NULL) h_res = p_fx_->SetTexture(h_param, NULL);
	else h_res = p_fx_->SetTexture(h_param, texture->get_d3d_texture());
	assert(SUCCEEDED(h_res));
	return SUCCEEDED(h_res);
}

bool GEREffect::set_matrix( const char* semantic, D3DXMATRIX &matrix )
{
	if (p_fx_ == NULL) return false;

	D3DXHANDLE h_param = NULL;
	h_param = p_fx_->GetParameterBySemantic(NULL, semantic);
	if (h_param == NULL) return false;

	HRESULT h_res = S_OK;
	h_res = p_fx_->SetMatrix(h_param, &matrix);
	assert(SUCCEEDED(h_res));
	return SUCCEEDED(h_res);
}

void GEREffect::on_lost_device()
{
	if(p_fx_)
	{
		p_fx_->OnLostDevice();
	}
}

void GEREffect::on_reset_device()
{
	if(p_fx_)
	{
		p_fx_->OnResetDevice();
	}
}

}

