#include "geo_text.h"
#include "../../common/ge_engine.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GEOText);

GEOText::GEOText()
: font_obj_(NULL)
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

void GEOText::render( time_t delta )
{

}

bool GEOText::set_font( GEFont* font )
{
	if (font == NULL) return false;
	font_obj_ = font;
	return true;
}

GEFont* GEOText::get_font()
{
	return font_obj_;
}

}

