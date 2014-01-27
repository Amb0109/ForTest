#include "ge_texture_manager.h"
#include "ge_texture.h"

namespace ge
{

GETextureManager::GETextureManager()
: texture_key_map_()
, texture_ref_map_()
{
}

GETextureManager::~GETextureManager()
{
}

GETextureManager* GETextureManager::get_instance()
{
	static GETextureManager _global_ge_texture_manager;
	return &_global_ge_texture_manager;
}

GETexture* GETextureManager::create_texture( const char* texture_path )
{
	GETextureManager* texture_manager = get_instance();
	if (texture_manager == NULL) return NULL;
	return texture_manager->_create_texture(texture_path);
}

void GETextureManager::release_texture( GETexture* texture )
{
	GETextureManager* texture_manager = get_instance();
	if (texture_manager == NULL) return;
	texture_manager->release_texture(texture);
}

GETexture* GETextureManager::_create_texture( const char* texture_path )
{
	TEXTURE_KEY_MAP::iterator itor_ptr = texture_key_map_.find(texture_path);
	if (itor_ptr == texture_key_map_.end()) {
		GETexture* texture = GETexture::create();
		texture = GETexture::create();
		if (texture) texture->create_texture(texture_path);
		texture_key_map_[texture_path] = texture;
		texture_ref_map_[texture] = 1;
		return texture;
	} else {
		GETexture* texture = itor_ptr->second;
		++ texture_ref_map_[texture];
		return texture;
	}
}

void GETextureManager::_release_texture( GETexture* texture )
{
	TEXTURE_REF_MAP::iterator itor_ref = texture_ref_map_.find(texture);
	if (itor_ref == texture_ref_map_.end()) return;
	int ref_cnt = -- itor_ref->second;
	if (ref_cnt == 0) {
		std::string texture_key = texture->get_key();
		texture_key_map_.erase(texture_key);

		texture->release_texture();
		delete texture;
	}
}



}