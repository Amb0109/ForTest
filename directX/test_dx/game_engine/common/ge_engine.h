#ifndef _GAME_ENGINE_ENGINE_H_
#define _GAME_ENGINE_ENGINE_H_

#include "ge_include.h"

namespace ge
{

class GE_API GED3DDeviceObject
{
public:
	virtual void on_lost_device() = 0;
	virtual void on_reset_device() = 0;
};

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

	virtual void register_device_object(GED3DDeviceObject*);
	virtual void unregister_device_object(GED3DDeviceObject*);

	virtual bool set_resolution(int width, int height);
	virtual bool set_windowed(bool is_windowed);
	virtual bool get_windowed();

protected:
	virtual bool _init_render();

	virtual bool _dx_begin_scene();
	virtual bool _dx_end_scene();
	virtual bool _dx_clear();
	virtual bool _dx_present();
	
	virtual bool _dx_check();
	virtual bool _dx_reset();
	virtual void _on_lost_device();
	virtual void _on_reset_device();
	virtual bool _on_dx_reset();

private:
	LPDIRECT3D9				p_d3d_;
	LPDIRECT3DDEVICE9		p_d3d_device_;
	D3DPRESENT_PARAMETERS	d3d_present_param_;

	GERender*				p_ge_render_;

	typedef std::set<GED3DDeviceObject*> DEVICE_OBJECT_SET;
	DEVICE_OBJECT_SET		device_object_set_;

	bool					is_windowed_;
};

} // namespace ge

#endif // _GAME_ENGINE_ENGINE_H_