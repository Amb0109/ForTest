#ifndef _GAME_ENGINE_COMMON_INPUT_H_
#define _GAME_ENGINE_COMMON_INPUT_H_

#include "../common/ge_include.h"

namespace ge
{
#define DEFINE_INPUT_DISL ( DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )
class GE_API GEInput
{
public:
	GEInput();
	virtual ~GEInput();

public:
	virtual bool init();
	virtual void destory();

	virtual void update();
	virtual bool update_picking();

public:
	virtual bool get_mouse_move(int& delta_x, int& delta_y, int& delta_z);
	virtual bool get_mouse_pos(int& pos_x, int& pos_y);

	virtual bool get_key_down(char key);
	virtual bool get_mouse_down(char button);

	virtual bool get_key_hold(char key);
	virtual bool get_mouse_hold(char button);

	virtual bool get_key_up(char key);
	virtual bool get_mouse_up(char button);

	virtual D3DXVECTOR3& get_picking_origin() { return packing_org_; }
	virtual D3DXVECTOR3& get_picking_direction() { return packing_dir_; }

private:
	LPDIRECTINPUT			p_input_;
	LPDIRECTINPUTDEVICE		p_keyboard_device_;
	char					keyboard_state_[256];
	char					keyboard_state_old_[256];

	LPDIRECTINPUTDEVICE		p_mouse_device_;
	DIMOUSESTATE2			mouse_state_;
	DIMOUSESTATE2			mouse_state_old_;

	D3DXVECTOR3				packing_org_;
	D3DXVECTOR3				packing_dir_;
};

}

#endif // _GAME_ENGINE_COMMON_INPUT_H_