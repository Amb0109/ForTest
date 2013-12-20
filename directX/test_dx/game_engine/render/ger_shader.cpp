#include "ger_shader.h"
#include "ge_render.h"
#include "../common/ge_engine.h"
#include "../common/ge_app.h"

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

	bool b_res = compile_shader_file(file_path, *this);
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
	SAFE_RELEASE(p_shader_);
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

bool GERShader::compile_shader_file( const char* file_path, GERShader& shader )
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

bool GERShader::compile_shader( const char* file_content, GERShader& shader )
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

