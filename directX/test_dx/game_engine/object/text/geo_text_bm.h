#ifndef _GAME_ENGINE_OBJECT_TEXT_BM_H_
#define _GAME_ENGINE_OBJECT_TEXT_BM_H_

#include "../ge_object.h"
#include "ge_bm_font.h"
#include "geo_text.h"

namespace ge
{

class GEOPrimitive;

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

	bool update_text();

public:
	virtual void render(time_t delta);

protected:
	void add_render_char(GE_TEXT_CHAR& text_char);
	void clear_render_chars();

private:
	typedef std::vector<GEOPrimitive*>	RENDER_OBJECT_LIST;
	typedef std::vector<GE_TEXT_CHAR>	RENDER_CHAR_LIST;

	RENDER_OBJECT_LIST			render_objectes_;
	RENDER_CHAR_LIST			render_chars_;

};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_BM_H_