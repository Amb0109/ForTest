#ifndef _GAME_ENGINE_OBJECT_SURFACE_H_
#define _GAME_ENGINE_OBJECT_SURFACE_H_

#include "../../common/ge_include.h"

namespace ge
{
class GE_API GESurface
{
	DLL_MANAGE_CLASS(GESurface);

public:
	GESurface();
	virtual ~GESurface();

public:
	virtual bool init(int width, int height, D3DFORMAT format = D3DFMT_UNKNOWN);
	virtual void destory();

	LPDIRECT3DSURFACE9 get_d3d_surface();
	void get_size(int& width, int& height);
	D3DFORMAT get_format();
	bool begin_dc(HDC& h_dc);
	bool end_dc(HDC& h_dc);

protected:
	LPDIRECT3DSURFACE9		d3d_surface_;
	D3DSURFACE_DESC			surface_desc_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_SURFACE_H_