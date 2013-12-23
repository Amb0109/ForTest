#include "scene_test.h"

SceneTest::SceneTest()
:p_test_model_(NULL),
p_fps_text_(NULL),
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

bool SceneTest::init_test_model()
{
	p_test_model_ = new ModelTest();
	if (p_test_model_ == NULL) return false;

	ge::GE_VERTEX_DECL vertex_decl_;
	vertex_decl_.init(DEF_FVF_FORMAT);
	p_test_model_->set_vertex_decl(&vertex_decl_);

	ge::GE_VERTEX vertex_buff[8];
	for (int i=0; i<8; ++i) vertex_buff[i].set_decl(&vertex_decl_);

	vertex_buff[0].set_position(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	vertex_buff[0].set_normal(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	vertex_buff[0].set_color(RGBA(0xff, 0xff, 0xff, 0xff));

	vertex_buff[1].set_position(D3DXVECTOR3(-1.0f, 1.0f, -1.0f));
	vertex_buff[1].set_normal(D3DXVECTOR3(-1.0f, 1.0f, -1.0f));
	vertex_buff[1].set_color(RGBA(0xff, 0x00, 0xff, 0xff));
	
	vertex_buff[2].set_position(D3DXVECTOR3(1.0f, 1.0f, -1.0f));
	vertex_buff[2].set_normal(D3DXVECTOR3(1.0f, 1.0f, -1.0f));
	vertex_buff[2].set_color(RGBA(0x00, 0x00, 0xff, 0xff));

	vertex_buff[3].set_position(D3DXVECTOR3(1.0f, -1.0f, -1.0f));
	vertex_buff[3].set_normal(D3DXVECTOR3(1.0f, -1.0f, -1.0f));
	vertex_buff[3].set_color(RGBA(0x00, 0xff, 0xff, 0xff));

	vertex_buff[4].set_position(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	vertex_buff[4].set_normal(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	vertex_buff[4].set_color(RGBA(0xff, 0xff, 0x00, 0xff));

	vertex_buff[5].set_position(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	vertex_buff[5].set_normal(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	vertex_buff[5].set_color(RGBA(0xff, 0x00, 0x00, 0xff));

	vertex_buff[6].set_position(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	vertex_buff[6].set_normal(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	vertex_buff[6].set_color(RGBA(0x00, 0x00, 0x00, 0xff));

	vertex_buff[7].set_position(D3DXVECTOR3(1.0f, -1.0f, 1.0f));
	vertex_buff[7].set_normal(D3DXVECTOR3(1.0f, -1.0f, 1.0f));
	vertex_buff[7].set_color(RGBA(0x00, 0xff, 0x00, 0xff));

	p_test_model_->set_vertices(vertex_buff, 8);

	WORD index_buff[36];
	index_buff[0] = 0; index_buff[1] = 1; index_buff[2] = 2;
	index_buff[3] = 0; index_buff[4] = 2; index_buff[5] = 3;
	index_buff[6] = 4; index_buff[7] = 6; index_buff[8] = 5;
	index_buff[9] = 4; index_buff[10] = 7; index_buff[11] = 6;
	index_buff[12] = 4; index_buff[13] = 5; index_buff[14] = 1;
	index_buff[15] = 4; index_buff[16] = 1; index_buff[17] = 0;
	index_buff[18] = 3; index_buff[19] = 2; index_buff[20] = 6;
	index_buff[21] = 3; index_buff[22] = 6; index_buff[23] = 7;
	index_buff[24] = 1; index_buff[25] = 5; index_buff[26] = 6;
	index_buff[27] = 1; index_buff[28] = 6; index_buff[29] = 2;
	index_buff[30] = 4; index_buff[31] = 0; index_buff[32] = 3;
	index_buff[33] = 4; index_buff[34] = 3; index_buff[35] = 7;
	p_test_model_->set_indices(index_buff, 36);

	object_map_[1] = p_test_model_;
	return true;
}

bool SceneTest::init_test_mesh()
{
	srand(clock());
	for (int i=0; i<10; ++i)
	{
		mesh_lst_.push_back(new MeshTest());
		mesh_lst_[i]->test_mesh_factory(rand()%5);
		
		switch (rand() % 3)
		{
		case 0:
			mesh_lst_[i]->set_material(&material0_);
			break;
		case 1:
			mesh_lst_[i]->set_material(&material1_);
			break;
		case 2:
			mesh_lst_[i]->set_material(&material2_);
			break;
		}

		object_map_[10 + i] = mesh_lst_[i];
	}

	return true;
}

bool SceneTest::init_material()
{
	material0_.set_color(0xffffffff, 0xff0f0ff0, 0xffffffff, 0xff000000);
	material0_.set_power(4.0f);
	material1_.set_color(0xffffffff, 0xfff00f0f, 0xffffffff, 0xff000000);
	material1_.set_power(4.0f);
	material2_.set_color(0xffffffff, 0xff0ff00f, 0xffffffff, 0xff000000);
	material2_.set_power(4.0f);
	ge::GE_MATERIAL::set_default_material(material0_);
	return true;
}

bool SceneTest::init_light()
{
	light_.set_type(D3DLIGHT_POINT);
	light_.set_position(5.f, 5.f, -5.f);
	light_.set_color(D3DXCOLOR(0xffffffff) * 0.4f,
		D3DXCOLOR(0xffffffff), D3DXCOLOR(0xffffffff) * 0.8f);
	light_.set_attenuation(1.0f, 0.f, 0.f);
	light_.set_range(100.f);
	light_.enable();
	return true;
}

bool SceneTest::show()
{
	init_fps_text();
	init_material();
	init_light();
	init_test_model();
	init_test_mesh();

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

	ge::GEInput* p_input = ge::GEApp::get_instance()->get_input();
	if (p_input && p_input->get_key_down(DIK_W))
	{
		ge::GERender* p_render = ge::GEEngine::get_instance()->get_render();
		if (p_render)
		{
			DWORD fill_mode = p_render->get_render_state(D3DRS_FILLMODE);
			p_render->set_render_state(D3DRS_FILLMODE, fill_mode % 3 + 1);
		}
	}


	ge::GEScene::update(time_elapsed);
}

