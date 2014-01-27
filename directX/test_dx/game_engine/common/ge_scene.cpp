#include "ge_scene.h"
#include "../common/ge_engine.h"
#include "../object/ge_object.h"
#include "../render/ge_render.h"

namespace ge
{

GEScene::GEScene()
{

}

GEScene::~GEScene()
{
	destory();
}

bool GEScene::init()
{
	object_map_.clear();
	return true;
}

void GEScene::destory()
{
	FOR_EACH (GE_OBJECT_MAP, object_map_, obj_it)
	{
		GEObject* p_obj = (GEObject*)(obj_it->second);
		if (NULL == p_obj) continue;
		p_obj->release();
		obj_it->second = NULL;
	}
	object_map_.clear();
}

void GEScene::add_object( int key, GEObject* obj )
{
	object_map_[key] = obj;
}

void GEScene::remove_object( int key )
{
	object_map_.erase(key);
}

bool GEScene::show()
{
	return true;
}

bool GEScene::hide()
{
	return true;
}

void GEScene::update( time_t delta )
{
	FOR_EACH (GE_OBJECT_MAP, object_map_, obj_it)
	{
		GEObject* p_obj = (GEObject*)(obj_it->second);
		if (NULL == p_obj) continue;
		p_obj->update(delta);
	}
}

void GEScene::render( time_t delta )
{
	GERender* p_render = GERender::get_instance();
	if (p_render == NULL) return;

	FOR_EACH (GE_OBJECT_MAP, object_map_, obj_it)
	{
		GEObject* p_obj = (GEObject*)(obj_it->second);
		if (NULL == p_obj) continue;
		p_render->push_render(p_obj);
	}
}

}