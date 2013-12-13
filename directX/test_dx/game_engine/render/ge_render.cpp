#include "ge_render.h"
#include "../common/ge_app.h"

namespace ge
{

GERender::GERender( LPDIRECT3DDEVICE9 p_d3d_device )
:p_d3d_device_(p_d3d_device),
font_manager_(p_d3d_device)
{
	GE_FONT ge_font = {
		"consolas",
		15,
		DEFAULT_CHARSET,
		400,
		false
	};

	def_font_id_ = font_manager_.add_font(ge_font);
}

GERender::~GERender()
{

}

void GERender::render()
{
	GE_IRECT rect(0, 0, 100, 100);
	GE_TEXT_STYLE style = {
		def_font_id_,
		DT_LEFT | DT_TOP,
		RGBA(0xff, 0xff, 0xff, 0xff),
		false,
		false,
		false, 0, RGBA(0xff, 0xff, 0xff, 0xff),
		false, 0, RGBA(0xff, 0xff, 0xff, 0xff)
	};
	float fps = GEApp::get_instance()->get_fps();
	char buff[32];
	sprintf_s(buff, "fps %.2f", fps);
	draw_text(buff, rect, style);
	return;
}

void GERender::release()
{
}

bool GERender::draw_text( const char* text, GE_IRECT& rect, GE_TEXT_STYLE& style )
{
	if (text == NULL) return false;
	if (rect.width() < 0 || rect.height() < 0) return false;
	ID3DXFont* font_obj = font_manager_.get_font(style.font_id);
	if (font_obj == NULL) return false;
	font_obj->DrawText(NULL, text, -1, &rect, style.format, style.font_color);
	return true;
}

}

