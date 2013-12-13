#include "ge_game.h"
#include "ge_scene.h"

namespace ge
{

GEGame::GEGame()
:cur_scene_id_(-1)
{
}

GEGame::~GEGame()
{
	remove_all_scenes();
}

GEGame* GEGame::get_instance()
{
	static GEGame _global_p_ge_game;
	return &_global_p_ge_game;
}

void GEGame::process( time_t time_elapsed )
{
	if(!is_valid_scene(cur_scene_id_))
	{
		cur_scene_id_ = -1;
		return;
	}

	scene_vec_[cur_scene_id_]->update(time_elapsed);
	scene_vec_[cur_scene_id_]->render(time_elapsed);
}

int GEGame::add_scene( GEScene* ge_scene )
{
	if(NULL == ge_scene) return -1;

	ge_scene->init();
	scene_vec_.push_back(ge_scene);
	int new_scene_id = (int)scene_vec_.size() - 1;
	if (cur_scene_id_ == -1) cur_scene_id_ = new_scene_id;

	return new_scene_id;
}

bool GEGame::switch_scene( int scene_id )
{
	if(!is_valid_scene(scene_id)) return false;

	if (is_valid_scene(cur_scene_id_))
	{
		if(!scene_vec_[cur_scene_id_]->hide())
			return false;
	}

	cur_scene_id_ = scene_id;
	return scene_vec_[cur_scene_id_]->show();
}

bool GEGame::is_valid_scene( int scene_id )
{
	if (0 > scene_id) return false;
	if (scene_id >= (int)scene_vec_.size()) return false;
	if (scene_vec_[scene_id] == NULL) return false;
	return true;
}

void GEGame::remove_scene( int scene_id )
{
	if (is_valid_scene(scene_id))
	{
		scene_vec_[scene_id]->destory();
		scene_vec_[scene_id] = NULL;

		if (cur_scene_id_ == scene_id)
		{
			cur_scene_id_ = scene_id;	
		}
	}
}

void GEGame::remove_all_scenes()
{
	int scene_cnt = (int)scene_vec_.size();
	for (int i=0; i<scene_cnt; ++i)
	{
		remove_scene(i);
	}
}

GEScene* GEGame::get_scene( int scene_id )
{
	if (is_valid_scene(scene_id))
	{
		return scene_vec_[scene_id];
	}
	return NULL;
}

}