#ifndef _GAME_ENGINE_OBJECT_TEXTURE_H_
#define _GAME_ENGINE_OBJECT_TEXTURE_H_

#include "../../common/ge_include.h"
#include "ge_surface.h"

namespace ge
{

class GE_API GETexture : public GESurface
{
	DLL_MANAGE_CLASS(GETexture);

public:
	GETexture();
	virtual ~GETexture();

public:
	virtual bool init(const char* texture_path);
	virtual bool init(int width, int height, D3DFORMAT format = D3DFMT_UNKNOWN);
	virtual void destory();
	
	virtual bool use_texture();
	static bool use_null_texture();

	LPDIRECT3DTEXTURE9 get_d3d_texture();
	const char* get_key();

protected:
	std::string				texture_key_;
	LPDIRECT3DTEXTURE9		d3d_texture_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXTURE_H_