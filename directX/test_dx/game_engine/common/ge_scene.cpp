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
		p_obj->destory();
		delete p_obj;
		obj_it->second = NULL;
	}
	object_map_.clear();
}

bool GEScene::show()
{
	return true;
}

bool GEScene::hide()
{
	return true;
}

void GEScene::update( time_t time_elapsed )
{
	FOR_EACH (GE_OBJECT_MAP, object_map_, obj_it)
	{
		GEObject* p_obj = (GEObject*)(obj_it->second);
		if (NULL == p_obj) continue;
		p_obj->update(time_elapsed);
	}
}

void GEScene::render( time_t time_elapsed )
{
	GEEngine* p_engine = GEEngine::get_instance();
	if (p_engine == NULL) return;
	GERender* p_render = p_engine->get_render();
	if (p_render == NULL) return;

	FOR_EACH (GE_OBJECT_MAP, object_map_, obj_it)
	{
		GEObject* p_obj = (GEObject*)(obj_it->second);
		if (NULL == p_obj) continue;
		p_render->push_render(p_obj);
	}
}

}