#include "game_engine.h"
#include "scene/scene_test.h"
#include "CCInclude.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static ge::GEApp* g_p_ge_app = NULL;
	static ge::GEEngine* g_p_ge_engine = NULL;
	static ge::GEGame* g_p_ge_game = NULL;
	g_p_ge_app = ge::GEApp::get_instance();
	g_p_ge_engine = ge::GEEngine::get_instance();
	g_p_ge_game = ge::GEGame::get_instance();

	bool init_ok = true;
	init_ok = init_ok && g_p_ge_app->create_app(hInstance, "Hello World!", 800, 600);
	init_ok = init_ok && g_p_ge_engine->init_engine();

	if (init_ok)
	{
		g_p_ge_game->add_scene(new SceneTest());

		g_p_ge_app->MainLoop();
	}

	g_p_ge_game->remove_all_scenes();
	g_p_ge_engine->close_engine();
	return 0;
}