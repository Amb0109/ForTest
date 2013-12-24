#include "scene_test.h"

SceneTest::SceneTest()
:p_fps_text_(NULL),
fps_font_id_(-1)
{
}

bool SceneTest::init_fps_text()
{
	p_fps_text_ = new ge::GEOText();
	if (p_fps_text_ == NULL) return false;

	ge::GE_FONT ge_font = {"consolas", 16, DEFAULT_CHARSET, 0, false };
	ge::GEEngine* p_engine = ge::GEEngine::get_instance();
	if (p_engine == NULL) return false;
	ge::GERFontManager* p_font_manager = p_engine->get_font_manager();
	if (p_font_manager == NULL) return false;

	fps_font_id_ = p_font_manager->add_d3dx_font(ge_font);
	ge::GE_TEXT_STYLE style = {
		fps_font_id_,
		DT_LEFT | DT_TOP,
		RGBA(0xff, 0xff, 0xff, 0xff),
		false, 0, RGBA(0xff, 0xff, 0xff, 0xff),
		false, 0, RGBA(0xff, 0xff, 0xff, 0xff)
	};
	p_fps_text_->set_text_style(style);

	ge::GE_IRECT rect(0, 0, 400, 200);
	p_fps_text_->set_rect(rect);

	object_map_[0] = p_fps_text_;
	return true;
}


bool SceneTest::show()
{
	init_fps_text();

	p_panel_2d_ = new Panel2D();
	p_panel_2d_->init();
	object_map_[1] = p_panel_2d_;

	return true;
}


bool SceneTest::hide()
{
	return true;
}

void SceneTest::update( time_t time_elapsed )
{
	if(NULL != p_fps_text_)
	{
		ge::GEApp* p_app = ge::GEApp::get_instance();
		float fps = p_app->get_fps();
		
		int mouse_x, mouse_y;
		p_app->get_input()->get_mouse_pos(mouse_x, mouse_y);

		char buff[1024];
		sprintf_s(buff, "fps: %.2f\nmouse: %d, %d", fps, mouse_x, mouse_y);
		p_fps_text_->set_text(buff);
	}

	ge::GEScene::update(time_elapsed);
}

