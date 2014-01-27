#ifndef _GAME_ENGINE_OBJECT_TEXTURE_MANAGER_H_
#define _GAME_ENGINE_OBJECT_TEXTURE_MANAGER_H_

#include "../../common/ge_include.h"
#include "ge_texture.h"

namespace ge
{

class GE_API GETextureManager
{
protected:
	GETextureManager();
	virtual ~GETextureManager();

	static GETextureManager* get_instance();

public:
	static GETexture* create_texture(const char* texture_path);
	static void release_texture(GETexture* texture);

	virtual GETexture* _create_texture(const char* texture_path);
	virtual void _release_texture(GETexture* texture);

protected:
	typedef std::map<std::string, GETexture*> TEXTURE_KEY_MAP;
	typedef std::map<GETexture*, int> TEXTURE_REF_MAP;

	TEXTURE_KEY_MAP		texture_key_map_;
	TEXTURE_REF_MAP		texture_ref_map_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXTURE_MANAGER_H_