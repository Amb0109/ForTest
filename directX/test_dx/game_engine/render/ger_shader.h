#ifndef _GAME_ENGINE_RENDER_SHADER_H_
#define _GAME_ENGINE_RENDER_SHADER_H_

#include "../common/ge_include.h"

namespace ge
{

const char* const SHADER_MAIN_FUNC	= "ShaderMain";
const char* const SHADER_TARGET		= "vs_2_0";

class GE_API GERShader
{
public:
	GERShader();
	virtual ~GERShader();

protected:
	static bool compile_shader_file(const char* file_path, GERShader& shader);
	static bool compile_shader(const char* file_content, GERShader& shader);

public:
	bool init(const char* file_path);
	void destory();
	const char* get_compile_error();

	LPD3DXCONSTANTTABLE get_constent_table() { return p_constent_table_; }
	LPDIRECT3DVERTEXSHADER9	get_shader_obj() { return p_shader_; }
	D3DXHANDLE get_value(const char* value_name, D3DXHANDLE content = NULL);

protected:
	LPD3DXCONSTANTTABLE		p_constent_table_;
	LPD3DXBUFFER			p_shader_buff_;
	LPD3DXBUFFER			p_error_message_;

	LPDIRECT3DVERTEXSHADER9	p_shader_;

	std::string				file_path_;
};


} // namespace ge

#endif // _GAME_ENGINE_RENDER_SHADER_H_