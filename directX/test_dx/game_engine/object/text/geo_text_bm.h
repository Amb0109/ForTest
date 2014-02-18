#ifndef _GAME_ENGINE_OBJECT_TEXT_BM_H_
#define _GAME_ENGINE_OBJECT_TEXT_BM_H_

#include "../ge_object.h"
#include "ge_bm_font.h"
#include "geo_text.h"
#include "../geo_atlas_render.h"

namespace ge
{

class GEOAtlasRender;

class GE_API GEOTextBM : public GEOText
{
	friend GEBMFont;

	DLL_MANAGE_CLASS(GEOTextBM);

public:
	GEOTextBM();
	virtual ~GEOTextBM();

public:
	static const unsigned fvf;

	virtual bool set_font(GEFont* font);
	virtual bool set_text(const char* text);

public:
	virtual void render(time_t delta);

protected:
	bool _init_text();
	void _add_render_char(GE_TEXT_CHAR& text_char);
	void _clear_render_chars();
	void _text_char_to_quad(GE_QUAD& out_quad, const GE_TEXT_CHAR& text_char);

private:
	typedef std::vector<GE_TEXT_CHAR>	RENDER_CHAR_LIST;

	GEOAtlasRender*				render_object_;
	RENDER_CHAR_LIST			render_chars_;

};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_BM_H_