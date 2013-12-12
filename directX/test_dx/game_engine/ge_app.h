#ifndef _GAME_ENGINE_APP_H_
#define _GAME_ENGINE_APP_H_

#include "ge_include.h"
#include "ge_def.h"

namespace ge
{

class GE_API GEApp
{
public:
	GEApp();

	virtual LRESULT	MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bUseDefWindowsProc);
	virtual LRESULT	MainLoop();
	virtual BOOL	PreTranslateMessage(MSG* uMsg) {return TRUE;}

public:
	virtual bool create_app(HINSTANCE h_instance, const char* wnd_title, int wnd_width, int wnd_height);
	virtual bool process() {return true;}

	bool		is_app_created()	{ return is_app_created_; }
	GE_IRECT&	get_game_rect()		{ return game_rect_; }
	HWND&		get_wnd()			{ return h_wnd_; }

protected:
	bool _calc_mid_wnd_pos(GE_IRECT& wnd_rect);

protected:
	HWND		h_wnd_;
	HACCEL		h_accel_;
	GE_IRECT	game_rect_;
	bool		is_app_created_;
};
static GEApp* _global_p_ge_app = NULL;

} // namespace ge

#endif // _GAME_ENGINE_APP_H_