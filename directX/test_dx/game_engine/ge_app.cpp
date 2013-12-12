#include "ge_app.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(ge::_global_p_ge_app)
	{
		BOOL use_def_win_proc = TRUE;
		LRESULT ge_win_proc_ret = 0;
		ge_win_proc_ret = ge::_global_p_ge_app->MsgProc( uMsg, wParam, lParam, use_def_win_proc );
		if (!use_def_win_proc) return ge_win_proc_ret;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


namespace ge
{

GEApp::GEApp()
	:is_app_created_(false)
{
	_global_p_ge_app = this;	
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

bool GEApp::create_app( HINSTANCE h_instance, const char* title, int width, int height )
{
	WNDCLASS wnd_class;
	ZeroMemory(&wnd_class, sizeof(wnd_class));
	wnd_class.lpszClassName	= "DxApp Window";
	wnd_class.lpfnWndProc	= (WNDPROC) WndProc;
	wnd_class.style			= CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	wnd_class.hInstance		= h_instance;
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
		0L, 0L, h_instance, 0L);
	if (h_wnd_ == NULL) return false;

	GetClientRect(h_wnd_, &game_rect_);
	UpdateWindow(h_wnd_);
	ShowWindow(h_wnd_, SW_NORMAL);
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


}
