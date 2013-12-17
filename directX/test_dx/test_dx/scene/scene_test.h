#ifndef _SCENE_SCENE_TEST_H_
#define _SCENE_SCENE_TEST_H_

#include "game_engine.h"
#include "../object/model_test.h"

class SceneTest : public ge::GEScene
{
public:
	SceneTest();

public:
	bool init_fps_text();
	bool init_test_model();

public:
	virtual bool show();
	virtual bool hide();

	virtual void update(time_t time_elapsed);

public:
	ModelTest*		p_test_model_;

	int				fps_font_id_;
	ge::GEOText*	p_fps_text_;
};

#endif //_SCENE_SCENE_TEST_H_