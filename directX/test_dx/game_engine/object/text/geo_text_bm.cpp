#include "geo_text_bm.h"
#include "../../common/ge_engine.h"
#include "../geo_primitive.h"

namespace ge
{
DLL_MANAGE_CLASS_IMPLEMENT(GEOTextBM);

const unsigned GEOTextBM::fvf = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

GEOTextBM::GEOTextBM()
{
}

GEOTextBM::~GEOTextBM()
{
}

bool GEOTextBM::set_font( GEFont* font )
{
	if (font == NULL) return false;
	if (font->type_ != FontType_D3DXFont) return false;
	bool ret = GEOText::set_font(font);
	if (ret)
	{
		//release_render_obj();

		GEBMFont* bm_font = (GEBMFont*)font_obj_;
		if (bm_font == NULL) return false;

		int png_cnt = bm_font->get_png_cnt();
		render_objectes_.assign(png_cnt, (GEOPrimitive*)NULL);
		for (int i=0; i<png_cnt; ++i)
		{
			render_objectes_[i] = GEOPrimitive::create();
			if (render_objectes_[i] != NULL) continue;
			char png_path[MAX_PATH];
			bm_font->get_png_path(png_path, i);
			render_objectes_[i]->create_texture(png_path);
		}
	}
	return ret;
}

bool GEOTextBM::set_text( const char* text )
{
	GEOText::set_text(text);
	update_text();
	return true;
}

void GEOTextBM::add_render_char( GE_TEXT_CHAR& text_char )
{
	render_chars_.push_back(text_char);
}

void GEOTextBM::clear_render_chars()
{
	render_chars_.clear();
}

bool GEOTextBM::update_text()
{
	GEBMFont* bm_font = (GEBMFont*)font_obj_;
	if (bm_font == NULL) return false;
	
	bool ret = bm_font->compose(this, text_.c_str(), 0, 0, false);
	if (ret)
	{
		for (unsigned i=0; i<render_chars_.size(); ++i)
		{
			GE_VERTEX verties[4];
			DWORD indices[6];

			//for (int i=0; i<n; ++i)
			//{
			//	//verties[i].set_decl(fvf);

			//}
		}
	}
	return ret;
}

void GEOTextBM::render( time_t delta )
{
	;
}

}

