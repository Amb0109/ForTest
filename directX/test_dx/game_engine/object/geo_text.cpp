#include "geo_text.h"
#include "../common/ge_engine.h"

namespace ge
{

GEOText::GEOText()
{

}

GEOText::~GEOText()
{

}

void GEOText::set_text( const char* text )
{
	if (text == NULL) return;
	text_ = text;
}

void GEOText::render( time_t time_elapsed )
{
	GEEngine* p_engine = GEEngine::get_instance();
	if (p_engine == NULL) return;
	GERFontManager* p_font_manager = p_engine->get_font_manager();
	if (p_font_manager == NULL) return;

	if (text_.length() == 0) return;
	if (text_rect_.width() <= 0 || text_rect_.height() <= 0) return;

	ID3DXFont* font_obj = p_font_manager->get_d3dx_font(text_style_.font_id);
	if (font_obj == NULL) return;

	font_obj->DrawText(NULL, text_.c_str(), -1, &text_rect_,
		text_style_.format, text_style_.font_color);
}


}

