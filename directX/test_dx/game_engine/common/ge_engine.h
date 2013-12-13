#ifndef _GAME_ENGINE_ENGINE_H_
#define _GAME_ENGINE_ENGINE_H_

#include "ge_include.h"

namespace ge
{

class GE_API GERender;

class GE_API GEEngine
{
public:
	GEEngine();
	virtual ~GEEngine();

	static GEEngine* get_instance();

public:
	virtual bool init_engine();
	virtual void close_engine();

	virtual void process(time_t time_elapsed);
	virtual bool dx_begin_scene();
	virtual bool dx_end_scene();
	virtual bool dx_clear();
	virtual bool dx_present();

protected:
	LPDIRECT3D9				p_d3d_;
	LPDIRECT3DDEVICE9		p_d3d_device_;
	D3DPRESENT_PARAMETERS	d3d_present_param_;

	GERender*				p_ge_render_;
};

} // namespace ge

#endif // _GAME_ENGINE_ENGINE_H_