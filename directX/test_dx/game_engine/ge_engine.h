#ifndef _GAME_ENGINE_ENGINE_H_
#define _GAME_ENGINE_ENGINE_H_

#include "ge_include.h"

namespace ge
{

class GE_API GEApp;
class GE_API GEEngine
{
public:
	GEEngine(GEApp* g_p_ge_app);
	virtual ~GEEngine();

public:
	bool init_engine();
	void close_engine();

protected:
	GEApp*					p_ge_app_;

	LPDIRECT3D9				p_d3d_;
	LPDIRECT3DDEVICE9		p_d3d_device_;
	D3DPRESENT_PARAMETERS	d3d_present_param_;

};

} // namespace ge

#endif // _GAME_ENGINE_ENGINE_H_