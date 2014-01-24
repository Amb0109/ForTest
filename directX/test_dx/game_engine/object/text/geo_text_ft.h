#ifndef _GAME_ENGINE_OBJECT_TEXT_FT_H_
#define _GAME_ENGINE_OBJECT_TEXT_FT_H_

#include "../ge_object.h"
#include "../../utility/geu_gmath.h"
#include "geo_text.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace ge
{

class GE_API GEFreeType
{
public:
	GEFreeType();
	virtual ~GEFreeType();

	static GEFreeType* get_instance();

public:
	bool init();
	void destory();

	FT_Face create_face(const char* font_name);
	void release_face(FT_Face ft_face);

private:
	FT_Library freetype_;
};


class GE_API GEOTextFT : public GEOText
{
	DLL_MANAGE_CLASS(GEOTextFT);

public:
	GEOTextFT();
	virtual ~GEOTextFT();

public:
	//virtual void render(time_t delta);

	//virtual bool set_rect(GE_IRECT& rect);
	//virtual bool set_text_style(GE_TEXT_STYLE& style, const char* font_path);
	virtual bool set_text(const char* text);
	bool set_font(GE_FONT& font);
	
	

private:
	FT_Face						ft_face_;

	typedef struct GLYPH_STRUCT_
	{
		FT_UInt		index;
		FT_Vector	pos;
		FT_Glyph	image;
	} GLYPH_STRUCT;

	std::vector<GLYPH_STRUCT>	glyph_buff_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_FT_H_