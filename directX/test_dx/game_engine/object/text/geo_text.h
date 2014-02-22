#ifndef _GAME_ENGINE_OBJECT_TEXT_H_
#define _GAME_ENGINE_OBJECT_TEXT_H_

#include "../ge_object.h"
#include "../../render/font/ge_font.h"

namespace ge
{

struct GE_API GE_TEXT_STYLE
{
	int			font_id;
	int			format;
	D3DCOLOR	font_color;
	bool		border;
	int			border_weight;
	D3DCOLOR	border_color;
	bool		shadow;
	int			shadow_weight;
	D3DCOLOR	shadow_color;
};

class GE_API GEOText : public GEObject
{
	DLL_MANAGE_CLASS(GEOText);

public:
	GEOText();
	virtual ~GEOText();

	virtual bool set_font(GEFont* font);
	virtual GEFont* get_font();

	virtual bool set_text(const char* text);
	virtual const char* get_text();

public:
	virtual void render(time_t delta);

public:
	std::string					text_;
	GEFont*						font_obj_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_H_