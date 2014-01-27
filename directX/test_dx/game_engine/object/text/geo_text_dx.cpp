#include "geo_text_dx.h"
#include "../../common/ge_engine.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GEOTextDX);

GEOTextDX::GEOTextDX()
{

}

GEOTextDX::~GEOTextDX()
{
	text_.clear();
}

bool GEOTextDX::set_rect( GE_IRECT& rect )
{
	text_rect_ = rect;
	return true;
}

bool GEOTextDX::set_font( GEFont* font )
{
	if (font == NULL) return false;
	if (font->type_ != FontType_D3DXFont) return false;
	return GEOText::set_font(font);
}

bool GEOTextDX::set_text_style( GE_TEXT_STYLE& style )
{
	text_style_ = style;
	return true;
}

void GEOTextDX::render( time_t delta )
{
	GED3DXFont* font_obj = (GED3DXFont*)font_obj_;
	if (font_obj == NULL) return;

	LPD3DXFONT d3d_font = font_obj->get_d3dx_obj();
	if (d3d_font == NULL) return;

	if (text_.length() == 0) return;
	if (text_rect_.width() <= 0 || text_rect_.height() <= 0) return;

	d3d_font->DrawText(NULL, text_.c_str(), -1, &text_rect_,
		text_style_.format, text_style_.font_color);
}

}

