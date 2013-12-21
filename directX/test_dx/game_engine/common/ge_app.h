#ifndef _GAME_ENGINE_APP_H_
#define _GAME_ENGINE_APP_H_

#include "ge_include.h"
#include "ge_input.h"
#include "../utility/geu_gmath.h"


namespace ge
{

class GE_API GEGame;
class GE_API GEEngine;
class GE_API GEApp
{
public:
	GEApp();
	virtual ~GEApp();

	static GEApp* get_instance();

public:
	virtual LRESULT	MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bUseDefWindowsProc);
	virtual LRESULT	MainLoop();
	virtual BOOL	PreTranslateMessage(MSG* uMsg) {return TRUE;}

public:
	virtual bool create_app(HINSTANCE h_instance, const char* wnd_title, int wnd_width, int wnd_height);
	virtual void process();

	bool		is_app_created()	{ return is_app_created_; }
	GE_IRECT&	get_game_rect()		{ return game_rect_; }
	HWND&		get_wnd()			{ return h_wnd_; }
	HINSTANCE	get_app_inst()		{ return h_app_inst_; }		
	float		get_fps()			{ return fps_; }

	GEInput*	get_input()			{ return &input_; };

protected:
	bool		_calc_mid_wnd_pos(GE_IRECT& wnd_rect);
	void		_update_time();

protected:
	HWND		h_wnd_;
	HACCEL		h_accel_;
	HINSTANCE	h_app_inst_;
	GE_IRECT	game_rect_;
	bool		is_app_created_;

	float		fps_;
	float		fps_elapsed_;
	int			frame_cnt_;
	time_t		cur_time_;
	time_t		last_time_;
	time_t		time_elapsed_;

	GEGame*		p_ge_game_;
	GEEngine*	p_ge_engine_;

	GEInput		input_;
	int			mouse_x_;
	int			mouse_y_;
};

} // namespace ge

#endif // _GAME_ENGINE_APP_H_