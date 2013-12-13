#ifndef _GAME_ENGINE_RENDER_H_
#define _GAME_ENGINE_RENDER_H_

#include "../common/ge_include.h"
#include "../utility/geu_gmath.h"
#include "ger_font.h"

namespace ge
{

class GE_API GERender
{
public:
	GERender(LPDIRECT3DDEVICE9 p_d3d_device);
	virtual ~GERender();

public:
	void render();
	void release();

	bool draw_text(const char* text, GE_IRECT& rect, GE_TEXT_STYLE& style);

protected:
	LPDIRECT3DDEVICE9	p_d3d_device_;

	GERFontManager		font_manager_;
	int					def_font_id_;
};

} // namespace ge

#endif // _GAME_ENGINE_RENDER_H_