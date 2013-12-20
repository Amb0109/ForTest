#ifndef _GAME_ENGINE_RENDER_EFFECT_H_
#define _GAME_ENGINE_RENDER_EFFECT_H_

#include "../common/ge_include.h"

namespace ge
{

class GE_API GEObject;
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
	virtual bool init();
	virtual void render(GEObject* obj, time_t time_elapsed);
	virtual void destory();

protected:
	LPD3DXEFFECT			p_fx_;
	LPD3DXBUFFER			p_err_msg_;

	std::string				file_path_;
};


} // namespace ge

#endif // _GAME_ENGINE_RENDER_EFFECT_H_