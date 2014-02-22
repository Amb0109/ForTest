#ifndef _GAME_ENGINE_OBJECT_TEXT_DX_H_
#define _GAME_ENGINE_OBJECT_TEXT_DX_H_

#include "../ge_object.h"
#include "../../utility/geu_gmath.h"
#include "../../render/font/ge_font.h"
#include "geo_text.h"

namespace ge
{
class GE_API GEOTextDX : public GEOText
{
	DLL_MANAGE_CLASS(GEOTextDX);

public:
	GEOTextDX();
	virtual ~GEOTextDX();

	virtual bool set_font(GEFont* font);

	virtual bool set_rect(GE_IRECT& rect);
	virtual bool set_text_style(GE_TEXT_STYLE& style);

public:
	virtual void render(time_t delta);

public:
	GE_IRECT		text_rect_;
	GE_TEXT_STYLE	text_style_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_DX_H_