#include "ge_app.h"
#include "ge_engine.h"
#include "ge_game.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ge::GEApp* _g_p_ge_app = ge::GEApp::get_instance();
	if(_g_p_ge_app)
	{
		BOOL use_def_win_proc = TRUE;
		LRESULT ge_win_proc_ret = 0;
		ge_win_proc_ret = _g_p_ge_app->MsgProc( uMsg, wParam, lParam, use_def_win_proc );
		if (!use_def_win_proc) return ge_win_proc_ret;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


namespace ge
{

GEApp::GEApp()
	:is_app_created_(false)
{
	p_ge_game_ = GEGame::get_instance();
	p_ge_engine_ = GEEngine::get_instance();
}

GEApp::~GEApp()
{
}

GEApp* GEApp::get_instance()
{
	static GEApp _global_p_ge_app;
	return &_global_p_ge_app;
}

LRESULT GEApp::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bUseDefWindowsProc )
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return 0;
}

LRESULT GEApp::MainLoop()
{
	MSG  msg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	h_accel_ = LoadAccelerators( NULL, MAKEINTRESOURCE(0) );

	while( WM_QUIT != msg.message  )
	{
		BOOL has_msg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

		if (has_msg)
		{
			if( 0 == TranslateAccelerator( h_wnd_, h_accel_, &msg ) )
			{
				if (PreTranslateMessage(&msg))
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
		}
		else
		{
			this->process();
		}
	}
	return 0;
}

bool GEApp::create_app( HINSTANCE h_app_inst, const char* title, int width, int height )
{
	h_app_inst_ = h_app_inst;

	WNDCLASS wnd_class;
	ZeroMemory(&wnd_class, sizeof(wnd_class));
	wnd_class.lpszClassName	= "DxApp Window";
	wnd_class.lpfnWndProc	= (WNDPROC) WndProc;
	wnd_class.style			= CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	wnd_class.hInstance		= h_app_inst;
	wnd_class.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wnd_class.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);
	wnd_class.lpszMenuName	= NULL;
	wnd_class.cbClsExtra	= 0;
	wnd_class.cbWndExtra	= 0;

	BOOL ret = TRUE;

	ret = RegisterClass( &wnd_class );
	if(!ret) return false;

	GE_IRECT wnd_rect = GE_IRECT(0, 0, width, height);
	ret = AdjustWindowRect(&wnd_rect, DEF_WND_STYLE, FALSE);
	if(!ret) return false;

	wnd_rect.move_to(0, 0);
	if(!this->_calc_mid_wnd_pos(wnd_rect))
		return false;

	h_wnd_ = CreateWindow("DxApp Window", title, DEF_WND_STYLE,
		wnd_rect.left, wnd_rect.top, wnd_rect.width(), wnd_rect.height(),
		0L, 0L, h_app_inst, 0L);
	if (h_wnd_ == NULL) return false;

	GetClientRect(h_wnd_, &game_rect_);
	UpdateWindow(h_wnd_);
	ShowWindow(h_wnd_, SW_NORMAL);
	input_.init();

	is_app_created_ = true;
	return true;
}

bool GEApp::_calc_mid_wnd_pos( GE_IRECT& wnd_rect )
{
	// 工作区大小
	GE_IRECT work_area_rect;
	if(!SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area_rect, 0))
		return false;

	int pos_x = (work_area_rect.width() - wnd_rect.width()) / 2;
	int pos_y = (work_area_rect.height() - wnd_rect.height()) / 2;
	wnd_rect.move_to(pos_x, pos_y);
	return true;
}

void GEApp::process()
{
	_update_time();
	input_.update();
	input_.update_picking();

	if (p_ge_game_ != NULL)
	{
		p_ge_game_->process(time_elapsed_);

		p_ge_engine_ = GEEngine::get_instance();
		if (p_ge_engine_ != NULL)
		{
			p_ge_engine_->process(time_elapsed_);
		}
	}
}

void GEApp::_update_time()
{
	last_time_ = cur_time_;
	cur_time_ = clock();
	time_elapsed_ = cur_time_ - last_time_;

	++ frame_cnt_;
	fps_elapsed_ += time_elapsed_ / 1000.f;

	if (fps_elapsed_ >= 1.0f)
	{
		fps_ = float(frame_cnt_) / fps_elapsed_;
		fps_elapsed_ = 0.f;
		frame_cnt_ = 0;
	}
}

}
