#include "ge_scene.h"
#include "../object/ge_object.h"

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
	FOR_EACH (GE_OBJECT_MAP, object_map_, obj_it)
	{
		GEObject* p_obj = (GEObject*)(obj_it->second);
		if (NULL == p_obj) continue;
		p_obj->render(time_elapsed);
	}
}

}