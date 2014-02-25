#include "ge_font_bm.h"
#include "../../object/text/geo_text_bm.h"
#include "../../render/ger_effect.h"
#include "../../render/ge_render.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GEFontBM);

GEFontBM::GEFontBM()
: bm_font_()
, effect_(NULL)
{
	type_ = FontType_BMFont;
}

GEFontBM::~GEFontBM()
{
	destory();
}

bool GEFontBM::init( const char* fnt_path )
{
	return (0 == bm_font_.Init(fnt_path));
}

void GEFontBM::destory()
{
	destory_effect();
}

bool GEFontBM::init_effect( const char* fx_path )
{
	if(!effect_) effect_ = GEREffect::create();
	if (effect_)
	{
		effect_->create_from_file(fx_path);

		GERender* render = GERender::get_instance();
		effect_->set_matrix("VIEW", render->get_view_matrix());
		effect_->set_matrix("PROJECTION", render->get_proj_matrix());
	}
	return effect_ != NULL;
}

void GEFontBM::destory_effect()
{
	if (effect_) GEREffect::release(&effect_);
}

GEREffect* GEFontBM::get_effect()
{
	return effect_;
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
	if (wrap)
	{
		bm_font_.WriteBox(0.f, 0.f, 0.f, (float)width, text, strlen(text), bmfont::FONT_ALIGN_LEFT);
	}
	else
	{
		bm_font_.WriteML(0.f, 0.f, 0.f, text, strlen(text), bmfont::FONT_ALIGN_LEFT);
	}

	return true;
}


}

