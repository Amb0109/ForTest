#include "geo_text.h"
#include "../../common/ge_engine.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GEOText);

GEOText::GEOText()
{

}

GEOText::~GEOText()
{
	text_.clear();
}

bool GEOText::set_text( const char* text )
{
	if (text == NULL) text_.clear();
	else text_ = text;
	return true;
}

const char* GEOText::get_text()
{
	return text_.c_str();
}

bool GEOText::set_rect( GE_IRECT& rect )
{
	text_rect_ = rect;
	return true;
}

bool GEOText::set_text_style( GE_TEXT_STYLE& style )
{
	text_style_ = style;
	return true;
}

void GEOText::render( time_t delta )
{
	GERFontManager* p_font_manager = GERFontManager::get_instance();
	if (p_font_manager == NULL) return;

	if (text_.length() == 0) return;
	if (text_rect_.width() <= 0 || text_rect_.height() <= 0) return;

	ID3DXFont* font_obj = p_font_manager->get_d3dx_font(text_style_.font_id);
	if (font_obj == NULL) return;

	font_obj->DrawText(NULL, text_.c_str(), -1, &text_rect_,
		text_style_.format, text_style_.font_color);
}

}

