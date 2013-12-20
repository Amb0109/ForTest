#ifndef _GAME_ENGINE_RENDER_EFFECT_H_
#define _GAME_ENGINE_RENDER_EFFECT_H_

#include "../common/ge_include.h"

namespace ge
{

class GE_API GEREffect
{
public:
	GEREffect();
	virtual ~GEREffect();

public:
	bool init(const char* file_path);
	void release();
	
	const char*		get_err_msg();
	LPD3DXEFFECT	get_fx();

protected:
	LPD3DXEFFECT			p_fx_;
	LPD3DXBUFFER			p_err_msg_;

	std::string				file_path_;
};


} // namespace ge

#endif // _GAME_ENGINE_RENDER_EFFECT_H_