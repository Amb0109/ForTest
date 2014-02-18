#ifndef _GAME_ENGINE_OBJECT_TEXT_GDI_H_
#define _GAME_ENGINE_OBJECT_TEXT_GDI_H_

#include "../ge_object.h"
#include "../../utility/geu_gmath.h"
#include "ge_font.h"
#include "geo_text.h"

namespace ge
{

class GEOAtlasRender;

class GE_API GEOTextGDI : public GEOText
{
	DLL_MANAGE_CLASS(GEOTextGDI);

public:
	GEOTextGDI();
	virtual ~GEOTextGDI();

public:
	static const unsigned fvf;

	virtual bool set_font(GEFont* font);

	virtual bool set_rect(GE_IRECT& rect);
	virtual bool set_text_style(GE_TEXT_STYLE& style);

	virtual bool set_text(const char* text);

public:
	virtual void render(time_t delta);

protected:
	virtual bool _init_render_object();
	virtual bool _init_bitmap(int width, int height);
	virtual bool _init_dc(int width, int height);
	virtual void _destory_bitmap();
	virtual void _destory_dc();
	virtual bool _update_quad();
	virtual bool _update_text();

private:
	GE_IRECT		text_rect_;
	GE_TEXT_STYLE	text_style_;

	HBITMAP			h_bitmap_;
	HDC				h_dc_;

	GEOAtlasRender*	render_object_;

	bool			need_update_quad_;
	bool			need_update_text_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_GDI_H_