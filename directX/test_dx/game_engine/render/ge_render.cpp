#include "ge_render.h"
#include "../common/ge_app.h"
#include "../common/ge_engine.h"
#include "../object/ge_object.h"

namespace ge
{

GERender::GERender()
{
}

GERender::~GERender()
{

}

bool GERender::init()
{
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	bool b_res = true;
	b_res = b_res && do_view_trans(position, target, up);
	b_res = b_res && do_projection_trans(0.5f);
	b_res = b_res && set_render_state(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return b_res;
}

void GERender::render(time_t time_elapsed)
{
	while (!render_task_que_.empty())
	{
		GEObject* p_obj = render_task_que_.front();
		render_task_que_.pop();
		if(!p_obj) continue;
		p_obj->render(time_elapsed);
	}

	return;
}


bool GERender::do_view_trans( D3DXVECTOR3& position, D3DXVECTOR3& target, D3DXVECTOR3& up )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if (p_d3d_device == NULL) return false;

	D3DXMATRIX matrix;
	D3DXMatrixLookAtLH(&matrix, &position, &target, &up);

	HRESULT h_res = p_d3d_device->SetTransform(D3DTS_VIEW, &matrix);
	return SUCCEEDED(h_res);
}

bool GERender::do_projection_trans( float fovy )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if (p_d3d_device == NULL) return false;
	GE_IRECT& wnd_rect = GEApp::get_instance()->get_game_rect();

	D3DXMATRIX matrix;
	D3DXMatrixPerspectiveFovLH(
		&matrix,
		D3DX_PI * fovy,
		(float) wnd_rect.width() / wnd_rect.height(),
		1.0f, 1000.0f);
	HRESULT h_res = p_d3d_device->SetTransform(D3DTS_PROJECTION, &matrix);
	return SUCCEEDED(h_res);
}

bool GERender::set_render_state( D3DRENDERSTATETYPE type, DWORD value )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if (p_d3d_device == NULL) return false;

	HRESULT h_res = p_d3d_device->SetRenderState(type, value);
	return SUCCEEDED(h_res);
}

void GERender::release()
{
}

void GERender::push_render( GEObject* p_object )
{
	render_task_que_.push(p_object);
}

}

