#include "game_engine.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static ge::GEApp* g_p_ge_app = NULL;
	static ge::GEEngine* g_p_ge_engine = NULL;
	g_p_ge_app = new ge::GEApp();
	g_p_ge_engine = new ge::GEEngine(g_p_ge_app);
	
	bool init_ok = true;
	init_ok = init_ok && g_p_ge_app->create_app(hInstance, "Hello!", 800, 600);
	init_ok = init_ok && g_p_ge_engine->init_engine();

	if (init_ok) g_p_ge_app->MainLoop();

	delete g_p_ge_engine;
	delete g_p_ge_app;
}