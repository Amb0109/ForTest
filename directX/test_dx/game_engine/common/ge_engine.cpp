#include "ge_engine.h"
#include "ge_app.h"
#include "../render/ge_render.h"

namespace ge
{

GEEngine::GEEngine()
:p_d3d_(NULL) ,p_d3d_device_(NULL),
p_ge_render_(NULL)
{
}

GEEngine::~GEEngine()
{
	close_engine();
}

GEEngine* GEEngine::get_instance()
{
	static GEEngine _global_ge_engine;
	return &_global_ge_engine;
}

LPDIRECT3DDEVICE9 GEEngine::get_device()
{
	return get_instance()->p_d3d_device_;
}

bool GEEngine::init_engine()
{
	GEApp* p_ge_app_ = GEApp::get_instance();
	if (p_ge_app_ == NULL) return false;
	if (!p_ge_app_->is_app_created()) return false;

	HRESULT h_res = S_OK;

	p_d3d_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == p_d3d_) return false;

	UINT adapter_type	= D3DADAPTER_DEFAULT;
	D3DDEVTYPE dev_type	= D3DDEVTYPE_HAL;
	D3DCAPS9 d3d_caps;
	h_res = p_d3d_->GetDeviceCaps(adapter_type, dev_type, &d3d_caps);
	if (FAILED(h_res))
	{
		dev_type = D3DDEVTYPE_REF;
		h_res = p_d3d_->GetDeviceCaps(adapter_type, dev_type, &d3d_caps);
	}
	if (FAILED(h_res)) return false;

	LONG vertex_proc_type = 0L;
	if(d3d_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertex_proc_type = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertex_proc_type = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	GE_IRECT& wnd_rect = p_ge_app_->get_game_rect();
	ZeroMemory(&d3d_present_param_, sizeof(d3d_present_param_));
	d3d_present_param_.BackBufferWidth				= wnd_rect.width();
	d3d_present_param_.BackBufferHeight				= wnd_rect.height();
	d3d_present_param_.BackBufferFormat				= D3DFMT_X8R8G8B8;
	d3d_present_param_.BackBufferCount				= 2;
	d3d_present_param_.MultiSampleType				= D3DMULTISAMPLE_NONE;
	d3d_present_param_.MultiSampleQuality			= 0;
	d3d_present_param_.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	d3d_present_param_.hDeviceWindow				= p_ge_app_->get_wnd();
	d3d_present_param_.Windowed						= TRUE;
	d3d_present_param_.EnableAutoDepthStencil		= TRUE;
	d3d_present_param_.AutoDepthStencilFormat		= D3DFMT_D24X8;
	d3d_present_param_.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3d_present_param_.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	d3d_present_param_.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

	h_res = p_d3d_->CreateDevice(D3DADAPTER_DEFAULT,
		dev_type, p_ge_app_->get_wnd(), vertex_proc_type,
		&d3d_present_param_, &p_d3d_device_);

	if (FAILED(h_res)) return false;

	SAFE_RELEASE(p_d3d_);

	return _init_render();
}

void GEEngine::close_engine()
{
	SAFE_RELEASE(p_d3d_device_);
}

bool GEEngine::_dx_begin_scene()
{
	if (p_d3d_device_ == NULL) return false;

	HRESULT h_res = p_d3d_device_->BeginScene();
	return SUCCEEDED(h_res);
}

bool GEEngine::_dx_end_scene()
{
	if (p_d3d_device_ == NULL) return false;

	HRESULT h_res = p_d3d_device_->EndScene();
	return SUCCEEDED(h_res);
}

bool GEEngine::_dx_clear()
{
	if (p_d3d_device_ == NULL) return false;

	HRESULT h_res = p_d3d_device_->Clear(0,
		NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		RGBA(127, 127, 127, 255), 1.0f, 0L);
	return SUCCEEDED(h_res);
}

bool GEEngine::_dx_present()
{
	if (p_d3d_device_ == NULL) return false;

	HRESULT h_res = p_d3d_device_->Present(NULL, NULL, NULL, NULL);
	return SUCCEEDED(h_res);
}

bool GEEngine::_dx_reset()
{
	if (p_d3d_device_ == NULL) return false;
	HRESULT h_res = p_d3d_device_->Reset(&d3d_present_param_);
	return SUCCEEDED(h_res);
}

void GEEngine::process( time_t delta )
{
	if (p_d3d_device_ == NULL) return;

	if(!_dx_begin_scene()) return;
	if(!_dx_clear()) return;

	if (p_ge_render_ != NULL)
		p_ge_render_->render(delta);

	if(!_dx_end_scene()) return;
	if(!_dx_present()) return;
}

bool GEEngine::_init_render()
{
	if (p_d3d_device_ == NULL) return false;

	p_ge_render_ = GERender::get_instance();
	if(p_ge_render_ == NULL) return false;

	p_font_manager_ = GERFontManager::get_instance();
	if(p_ge_render_ == NULL) return false;

	return p_ge_render_->init();
}

}
