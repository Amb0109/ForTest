#include "ge_font_bm.h"
#include "../../object/text/geo_text_bm.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GEFontBM);

GEFontBM::GEFontBM()
: bm_font_()
{
	type_ = FontType_BMFont;
}

GEFontBM::~GEFontBM()
{

}

bool GEFontBM::init( const char* fnt_path )
{
	return (0 == bm_font_.Init(fnt_path));
}

int GEFontBM::get_page_cnt()
{
	return bm_font_.GetPageCount();
}

bool GEFontBM::get_page_path( char* page_path, int index )
{
	strcpy(page_path, bm_font_.GetPagePath(index).c_str());
	return true;
}
//
//GETexture* GEFontBM::get_texture( int index/* = 0*/ )
//{
//	return NULL;
//}

bool GEFontBM::begin_write( bmfont::SCharRenderObject* char_buff, int buff_size )
{
	if (char_buff == NULL) return false;
	if (buff_size <= 0) return false;
	return bm_font_.SetWriteBuff(char_buff, buff_size);
}

int GEFontBM::end_write()
{
	int ret_size = bm_font_.GetWriteCount();
	bm_font_.SetWriteBuff(NULL, 0);
	return ret_size;	
}

bool GEFontBM::write_text( const char* text, int width, int height, bool wrap )
{
	bm_font_.WriteBox(0.f, 0.f, 0.f,
		(float)width, text, strlen(text), bmfont::FONT_ALIGN_LEFT);
	return true;
}


}

