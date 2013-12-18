#include "ger_shader.h"
#include "ge_render.h"
#include "../common/ge_engine.h"

namespace ge
{


GERShader::GERShader()
:p_shader_buff_(NULL),
p_error_message_(NULL),
p_constent_table_(NULL),
p_shader_(NULL)
{

}

GERShader::~GERShader()
{

}

bool GERShader::create( const char* file_path )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;

	bool b_res = GERShaderFactory::compile_shader_file(file_path, *this);
	if (!b_res) return false;

	HRESULT h_res = p_d3d_device->CreateVertexShader(
		(DWORD*)p_shader_buff_->GetBufferPointer(),
		&p_shader_);
	return SUCCEEDED(h_res);
}

void GERShader::release()
{
	file_path_.clear();

	SAFE_RELEASE(p_shader_buff_);
	SAFE_RELEASE(p_error_message_);
	SAFE_RELEASE(p_constent_table_);
}



const char* GERShader::get_compile_error()
{
	if (p_error_message_ == NULL) return NULL;
	return (char*)p_error_message_->GetBufferPointer();
}

D3DXHANDLE GERShader::get_value( const char* value_name, D3DXHANDLE content /*= NULL*/ )
{
	if (p_constent_table_ == NULL) return NULL;
	return p_constent_table_->GetConstantByName(content, value_name);
}

void GERShader::test_func()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return ;

	GERender* p_render = GEEngine::get_instance()->get_render();
	if(p_render == NULL) return ;

	D3DXHANDLE h_wvm = get_value("g_world_view_matrix");
	D3DXHANDLE h_wvpm = get_value("g_world_view_proj_matrix");
	D3DXHANDLE h_color = get_value("g_color");
	D3DXHANDLE h_ld = get_value("g_light_direction");

	HRESULT h_res = S_OK;
	h_res = p_constent_table_->SetMatrix(p_d3d_device, h_wvm, &p_render->get_word_view_matrix());
	h_res = p_constent_table_->SetMatrix(p_d3d_device, h_wvpm, &p_render->get_word_view_proj_matrix());
	
	D3DXVECTOR4 color(0.3f, 0.7f, 0.4f, 0.5f);
	h_res = p_constent_table_->SetVector(p_d3d_device, h_color, &color);

	D3DXVECTOR4 light_direction_vec4(-0.57f, 0.57f, -0.57f, 0.0f);
	h_res = p_constent_table_->SetVector(p_d3d_device, h_ld, &light_direction_vec4);

	h_res = p_d3d_device->SetVertexShader(p_shader_);
}

bool GERShaderFactory::compile_shader_file( const char* file_path, GERShader& shader )
{
	shader.release();
	if (file_path == NULL) return false;

	shader.file_path_ = file_path;
	HRESULT h_res = D3DXCompileShaderFromFile(
		file_path, NULL, NULL,
		SHADER_MAIN_FUNC, SHADER_TARGET,
#ifdef _DEBUG
		D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
#else
		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
#endif
		&shader.p_shader_buff_,
		&shader.p_error_message_,
		&shader.p_constent_table_);

	return SUCCEEDED(h_res);
}

bool GERShaderFactory::compile_shader( const char* file_content, GERShader& shader )
{
	shader.release();
	if (file_content == NULL) return false;

	HRESULT h_res = D3DXCompileShader(
		file_content, (UINT)strlen(file_content), NULL, NULL,
		SHADER_MAIN_FUNC, SHADER_TARGET,
#ifdef _DEBUG
		D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION,
#else
		NULL,
#endif
		&shader.p_shader_buff_,
		&shader.p_error_message_,
		&shader.p_constent_table_);

	return SUCCEEDED(h_res);
}

}

