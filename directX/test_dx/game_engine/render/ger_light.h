#ifndef _GAME_ENGINE_OBJECT_LIGHT_H_
#define _GAME_ENGINE_OBJECT_LIGHT_H_

#include "../common/ge_include.h"

namespace ge
{

class GE_API GE_LIGHT
{
public:
	GE_LIGHT();
	explicit GE_LIGHT(D3DLIGHTTYPE type);
	virtual ~GE_LIGHT();

public:
	void set_type(D3DLIGHTTYPE type);
	void set_position(float pos_x, float pos_y, float pos_z);
	void set_direction(float dir_x, float dir_y, float dir_z);
	void set_color(D3DXCOLOR ambient_color, D3DXCOLOR diffuse_color, D3DXCOLOR specular_color);
	void set_range(float range);
	void set_attenuation(float atten0, float atten1, float atten2);
	void set_spotlight_info(float falloff, float theta, float phi);

	bool enable();
	bool disable();

protected:
	static int _get_light_index();

private:
	D3DLIGHT9		light_;
	int				light_index_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_LIGHT_H_