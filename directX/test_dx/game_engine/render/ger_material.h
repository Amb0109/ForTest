#ifndef _GAME_ENGINE_RENDER_MATERIAL_H_
#define _GAME_ENGINE_RENDER_MATERIAL_H_

#include "../common/ge_include.h"

namespace ge
{

class GE_API GE_MATERIAL
{
public:
	GE_MATERIAL();
	virtual ~GE_MATERIAL();

public:
	static void set_default_material(GE_MATERIAL& default_material);
	static GE_MATERIAL& get_default_material();
	static bool use_default_material();

public:
	void	set_ambient_color(D3DXCOLOR ambient_color);
	void	set_diffuse_color(D3DXCOLOR diffuse_color);
	void	set_specular_color(D3DXCOLOR specular_color);
	void	set_emissive_color(D3DXCOLOR emissive_color);
	void	set_color(	D3DXCOLOR ambient_color,
						D3DXCOLOR diffuse_color,
						D3DXCOLOR specular_color,
						D3DXCOLOR emissive_color);
	void	set_power(float power);

	D3DMATERIAL9* get_d3d_material() { return &d3d_material_; }

protected:
	D3DMATERIAL9		d3d_material_;
};

} // namespace ge

#endif // _GAME_ENGINE_RENDER_MATERIAL_H_