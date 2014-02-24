#ifndef _GAME_ENGINE_RENDER_EFFECT_H_
#define _GAME_ENGINE_RENDER_EFFECT_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"

namespace ge
{

class GE_API GEREffect
{
public:
	GEREffect();
	virtual ~GEREffect();

public:
	bool create_from_file(const char* file_path);

	const char*		get_err_msg();
	LPD3DXEFFECT	get_fx() { return p_fx_; }

public:
	virtual bool	init();
	virtual void	destory();

	virtual bool	set_technique(const char* name);
	virtual int		begin_effect();
	virtual bool	end_effect();
	virtual bool	begin_pass(int pass_id);
	virtual bool	end_pass();

protected:
	LPD3DXEFFECT			p_fx_;
	LPD3DXBUFFER			p_err_msg_;

	std::string				file_path_;
};


} // namespace ge

#endif // _GAME_ENGINE_RENDER_EFFECT_H_