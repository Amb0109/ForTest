#include "scene_test.h"

SceneTest::SceneTest()
: p_fps_text_(NULL)
, p_armature_(NULL)
, fps_font_id_(-1)
{
	position = D3DXVECTOR3(0.0f, 0.0f, -256.f);
	target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

bool SceneTest::init_fps_text()
{
	p_fps_text_ = ge::GEOTextDX::create();
	if (p_fps_text_ == NULL) return false;

	ge::GEFont* ge_font = ge::GEFontManager::create_font(ge::FontType_D3DXFont);
	if (ge_font == NULL) return false;
	ge_font->init("consolas", 16);
	ge_font->update_font();

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

	p_fps_text_->set_font(ge_font);

	add_object(0, p_fps_text_);
	return true;
}


bool SceneTest::show()
{
	ge::GEScene::show();

	init_fps_text();

	//p_panel_2d_ = new Panel2D();
	//p_panel_2d_->init();
	//add_object(1, p_panel_2d_);

	p_spine_ = ge::GEOSpine::create();
	p_spine_->init();
	//add_object(2, p_spine_);

	p_armature_ = ge::GEOArmature::create();
	p_armature_->init();
	//add_object(3, p_armature_);


	p_bm_font_ = new ge::GEBMFont();
	p_bm_font_->parse_binary_file("bmfont\\arial.fnt");

	return true;
}


bool SceneTest::hide()
{
	remove_object(2);
	ge::GEOSpine::release(&p_spine_);

	remove_object(3);
	ge::GEOArmature::release(&p_armature_);

	remove_object(0);
	ge::GEOTextDX::release(&p_fps_text_);

	return true;
}

void SceneTest::update( time_t time_elapsed )
{
	update_fps_text();
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

	ge::GERender* p_render = ge::GERender::get_instance();
	if (p_render == NULL) return;
	ge::GEInput* input = p_app->get_input();
	if (input == NULL) return;
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

void SceneTest::update_fps_text()
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
}
