#include "scene_test.h"

SceneTest::SceneTest()
:p_fps_text_(NULL),
fps_font_id_(-1)
{
	position = D3DXVECTOR3(0.0f, 0.0f, -256.f);
	target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
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

	add_object(0, p_fps_text_);
	return true;
}


bool SceneTest::show()
{
	init_fps_text();

	//p_panel_2d_ = new Panel2D();
	//p_panel_2d_->init();
	//add_object(1, p_panel_2d_);

	p_spine_ = new ge::GEOSpine();
	p_spine_->init();
	//add_object(2, p_spine_);

	p_armature_ = new ge::GEOArmature();
	p_armature_->init();
	add_object(3, p_armature_);

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
	
	update_camera();

	ge::GEScene::update(time_elapsed);
}

void SceneTest::render(time_t time_elapsed)
{
	ge::GEScene::render(time_elapsed);
}

void SceneTest::update_camera()
{
	ge::GEApp* p_app = ge::GEApp::get_instance();
	ge::GEEngine* p_engine = ge::GEEngine::get_instance();

	ge::GERender* p_render = p_engine->get_render();
	ge::GEInput* input = p_app->get_input();
	if (input == NULL) return;
	if (p_render == NULL) return;
	int delta_x = 0;
	int delta_y = 0;
	int delta_z = 0;
	input->get_mouse_move(delta_x, delta_y, delta_z);
	
	position.z += delta_z / 10;

	if (delta_z != 0 || input->get_mouse_hold(0))
	{
		position.x -= delta_x;
		position.y += delta_y;

		target.x -= delta_x;
		target.y += delta_y;

		p_render->do_view_trans(position, target, up);
	}

}
