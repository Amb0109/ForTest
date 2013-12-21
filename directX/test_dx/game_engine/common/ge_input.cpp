#include "ge_input.h"
#include "ge_engine.h"
#include "ge_app.h"

namespace ge
{
GEInput::GEInput()
:p_input_(NULL),
p_keyboard_device_(NULL), p_mouse_device_(NULL)
{

}

GEInput::~GEInput()
{
	destory();
}

bool GEInput::init()
{
	GEApp* p_app = GEApp::get_instance();
	if (p_app == NULL) return false;
	HINSTANCE h_app_inst = p_app->get_app_inst();
	if (h_app_inst == NULL) return false;
	HWND h_wnd = p_app->get_wnd();
	if (h_wnd == NULL) return false;

	HRESULT h_res = S_OK;
	h_res = DirectInput8Create(h_app_inst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&p_input_, NULL);

	if (FAILED(h_res)) return false;

	h_res = p_input_->CreateDevice(GUID_SysKeyboard, &p_keyboard_device_, NULL);
	h_res = p_keyboard_device_->SetDataFormat(&c_dfDIKeyboard);
	h_res = p_keyboard_device_->SetCooperativeLevel(h_wnd, DEFINE_INPUT_DISL);
	h_res = p_keyboard_device_->Acquire();

	h_res = p_input_->CreateDevice(GUID_SysMouse, &p_mouse_device_, 0);
	h_res = p_mouse_device_->SetDataFormat(&c_dfDIMouse2);
	h_res = p_mouse_device_->SetCooperativeLevel(h_wnd, DEFINE_INPUT_DISL);
	h_res = p_mouse_device_->Acquire();

	return SUCCEEDED(h_res);
}

void GEInput::destory()
{
	p_keyboard_device_->Unacquire();
	p_mouse_device_->Unacquire();

	SAFE_RELEASE(p_keyboard_device_);
	SAFE_RELEASE(p_mouse_device_);
	SAFE_RELEASE(p_input_);
}

void GEInput::update()
{
	HRESULT h_res = S_OK;
	if (p_keyboard_device_ != NULL)
	{
		memcpy(keyboard_state_old_, keyboard_state_, sizeof(keyboard_state_));
		h_res = p_keyboard_device_->GetDeviceState(sizeof(keyboard_state_), (void**)&keyboard_state_);
		if (FAILED(h_res))
		{
			h_res = p_keyboard_device_->Acquire();
			h_res = p_keyboard_device_->GetDeviceState(sizeof(keyboard_state_), (void**)&keyboard_state_);
			if (FAILED(h_res))
			{
				memset(keyboard_state_, 0, sizeof(keyboard_state_));
				h_res = p_keyboard_device_->Acquire();
			}
		}
	}

	if (p_mouse_device_ != NULL)
	{
		memcpy(&mouse_state_old_, &mouse_state_, sizeof(mouse_state_));
		h_res = p_mouse_device_->GetDeviceState(sizeof(mouse_state_), (void**)&mouse_state_);
		if (FAILED(h_res))
		{
			h_res = p_mouse_device_->Acquire();
			h_res = p_mouse_device_->GetDeviceState(sizeof(mouse_state_), (void**)&mouse_state_);
			if (FAILED(h_res))
			{
				memset(&mouse_state_, 0, sizeof(mouse_state_));
				h_res = p_mouse_device_->Acquire();
			}
		}
	}
}

bool ge::GEInput::get_mouse_move( int& delta_x, int& delta_y, int& delta_z )
{
	delta_x = mouse_state_.lX;
	delta_y = mouse_state_.lY;
	delta_z = mouse_state_.lZ;
	return true;
}

bool GEInput::get_mouse_pos( int& pos_x, int& pos_y )
{
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	
	HWND h_wnd = GEApp::get_instance()->get_wnd();
	if (h_wnd != NULL) ScreenToClient(h_wnd, &mouse_pos);

	pos_x = mouse_pos.x;
	pos_y = mouse_pos.y;
	return true;
}

bool GEInput::get_key_down(char key)
{
	bool cur_on = 0 != (keyboard_state_[(unsigned char)key] & 0x80);
	bool old_on = 0 != (keyboard_state_old_[(unsigned char)key] & 0x80);

	return (cur_on) && (!old_on);
}

bool GEInput::get_mouse_down( char button )
{
	bool cur_on = 0 != (mouse_state_.rgbButtons[(unsigned char)button] & 0x80);
	bool old_on = 0 != (mouse_state_.rgbButtons[(unsigned char)button] & 0x80);

	return (cur_on) && (!old_on);
}

bool GEInput::get_key_hold( char key )
{
	bool cur_on = 0 != (keyboard_state_[(unsigned char)key] & 0x80);
	bool old_on = 0 != (keyboard_state_old_[(unsigned char)key] & 0x80);

	return (cur_on) && (old_on);
}

bool GEInput::get_mouse_hold( char button )
{
	bool cur_on = 0 != (mouse_state_.rgbButtons[(unsigned char)button] & 0x80);
	bool old_on = 0 != (mouse_state_.rgbButtons[(unsigned char)button] & 0x80);

	return (cur_on) && (old_on);
}

bool GEInput::get_key_up( char key )
{
	bool cur_on = 0 != (keyboard_state_[(unsigned char)key] & 0x80);
	bool old_on = 0 != (keyboard_state_old_[(unsigned char)key] & 0x80);

	return (!cur_on) && (old_on);
}

bool GEInput::get_mouse_up( char button )
{
	bool cur_on = 0 != (mouse_state_.rgbButtons[(unsigned char)button] & 0x80);
	bool old_on = 0 != (mouse_state_.rgbButtons[(unsigned char)button] & 0x80);

	return (!cur_on) && (old_on);
}

}


