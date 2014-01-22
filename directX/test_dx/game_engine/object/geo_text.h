#ifndef _GAME_ENGINE_OBJECT_TEXT_H_
#define _GAME_ENGINE_OBJECT_TEXT_H_

#include "ge_object.h"
#include "../utility/geu_gmath.h"
#include "../render/ger_font.h"

namespace ge
{
class GE_API GEOText : public GEObject
{
	DLL_MANAGE_CLASS(GEOText);

	GEOText();
	virtual ~GEOText();

public:
	void set_text(const char* text);
	const char* get_text(){ return text_.c_str(); }

	void set_rect(GE_IRECT& rect) { text_rect_ = rect; }
	void set_text_style(GE_TEXT_STYLE& style) { text_style_ = style; };

public:
	virtual void render(time_t delta);

public:
	std::string		text_;
	GE_IRECT		text_rect_;
	GE_TEXT_STYLE	text_style_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_H_