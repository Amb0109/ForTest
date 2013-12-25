#ifndef _SCENE_SCENE_TEST_H_
#define _SCENE_SCENE_TEST_H_

#include "game_engine.h"
#include "../object/panel_2d.h"

class SceneTest : public ge::GEScene
{
public:
	SceneTest();

public:
	bool init_fps_text();

public:
	virtual bool show();
	virtual bool hide();

	virtual void update(time_t time_elapsed);

public:

	int				fps_font_id_;
	ge::GEOText*	p_fps_text_;

	Panel2D*		p_panel_2d_;

	ge::GEOSpine*	p_spine_;

};

#endif //_SCENE_SCENE_TEST_H_