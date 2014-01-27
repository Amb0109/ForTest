#ifndef _GAME_ENGINE_ENGINE_H_
#define _GAME_ENGINE_ENGINE_H_

#include "ge_include.h"

namespace ge
{

class GERender;
class GEFontManager;

class GE_API GEEngine
{
public:
	GEEngine();
	virtual ~GEEngine();

	static GEEngine*			get_instance();
	static LPDIRECT3DDEVICE9	get_device();

public:
	virtual bool init_engine();
	virtual void close_engine();

	virtual void process(time_t delta);

protected:
	virtual bool _init_render();

	virtual bool _dx_begin_scene();
	virtual bool _dx_end_scene();
	virtual bool _dx_clear();
	virtual bool _dx_present();
	virtual bool _dx_reset();

private:
	LPDIRECT3D9				p_d3d_;
	LPDIRECT3DDEVICE9		p_d3d_device_;
	D3DPRESENT_PARAMETERS	d3d_present_param_;

	GERender*				p_ge_render_;
	GEFontManager*			p_font_manager_;
};

} // namespace ge

#endif // _GAME_ENGINE_ENGINE_H_