#include "geo_text_ft.h"
#include "../../common/ge_engine.h"

namespace ge
{

GEFreeType::GEFreeType()
: freetype_(NULL)
{
}

GEFreeType::~GEFreeType()
{
	destory();
}

GEFreeType* GEFreeType::get_instance()
{
	static GEFreeType _global_ge_freetype;
	return &_global_ge_freetype;
}

bool GEFreeType::init()
{
	int ret = FT_Init_FreeType(&freetype_);
	if (ret != 0) return false;
	if (freetype_ == NULL) return false;

	return true;
}

void GEFreeType::destory()
{
	if (freetype_)
	{
		FT_Done_FreeType(freetype_);
		freetype_ = NULL;
	}
}

FT_Face GEFreeType::create_face( const char* font_path )
{
	FT_Face ft_face = NULL;
	FT_New_Face(freetype_, font_path, 0, &ft_face);
	return ft_face;
}

void GEFreeType::release_face( FT_Face ft_face )
{
	if (ft_face == NULL) return;
	FT_Done_Face(ft_face);
}


DLL_MANAGE_CLASS_IMPLEMENT(GEOTextFT);

GEOTextFT::GEOTextFT()
{

}

GEOTextFT::~GEOTextFT()
{

}

bool GEOTextFT::set_font( GE_FONT& font )
{
	GEFreeType::get_instance()->release_face(ft_face_);

	ft_face_ = GEFreeType::get_instance()->create_face(font.font_key);
	if (ft_face_ == NULL) return false;

	int ret = FT_Set_Pixel_Sizes(ft_face_, 0, font.font_size);
	return (ret == 0);
}

bool GEOTextFT::set_text( const char* text )
{
	if (ft_face_ == NULL) return false;

	GEOText::set_text(text);

	glyph_buff_.clear();

	FT_GlyphSlot slot = ft_face_->glyph;
	FT_UInt glyph_index = 0;
	FT_UInt pre_glyph_index = 0;
	FT_Bool use_kerning = false; 
	int pen_x = 0;
	int pen_y = 0;

	for (unsigned i=0; i<text_.size(); ++i)
	{
		glyph_index = FT_Get_Char_Index(ft_face_, text_[i]);
		
		int ret = FT_Load_Glyph(ft_face_, glyph_index, FT_LOAD_RENDER);
		if (ret != 0) continue;
		
		if ( use_kerning && pre_glyph_index && glyph_index )
		{
			FT_Vector delta;

			FT_Get_Kerning( ft_face_, pre_glyph_index, glyph_index, 
				FT_KERNING_DEFAULT, &delta );

			pen_x += delta.x >> 6;
		}

		/* 保存当前笔位置 */ 
		glyph_buff_[i].pos.x = pen_x;
		glyph_buff_[i].pos.y = pen_y;

		ret = FT_Load_Glyph(ft_face_, glyph_index, FT_LOAD_DEFAULT); 
		if ( ret != 0 ) continue;

		ret = FT_Get_Glyph(ft_face_->glyph, &glyph_buff_[i].image); 
		if ( ret != 0 ) continue;
	}
	return true;
}



}

