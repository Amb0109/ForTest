#ifndef _GAME_ENGINE_OBJECT_TEXTURE_H_
#define _GAME_ENGINE_OBJECT_TEXTURE_H_

#include "../../common/ge_include.h"

namespace ge
{

class GE_VERTEX;

class GE_API GETexture
{
	DLL_MANAGE_CLASS(GETexture);

public:
	GETexture();
	virtual ~GETexture();

public:
	virtual bool create_texture(const char* texture_path);
	virtual void release_texture();

	const char* get_key();

	virtual bool use_texture();
	static bool use_null_texture();

	virtual void get_texture_size(int& width, int& height);

protected:
	std::string				texture_key_;
	LPDIRECT3DTEXTURE9		d3d_texture_;
	D3DSURFACE_DESC			texture_desc_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXTURE_H_