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
	virtual void render(time_t time_elapsed);

	virtual void update_camera();
	virtual void update_fps_text();

public:

	int					fps_font_id_;
	ge::GEOText*		p_fps_text_;

	Panel2D*			p_panel_2d_;

	ge::GEOSpine*		p_spine_;

	ge::GEOArmature*	p_armature_;

	ge::GEBMFont*		p_bm_font_;

	D3DXVECTOR3			position;
	D3DXVECTOR3			target;
	D3DXVECTOR3			up;

};

#endif //_SCENE_SCENE_TEST_H_