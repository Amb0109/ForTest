#ifndef _GAME_ENGINE_RENDER_FONT_H_
#define _GAME_ENGINE_RENDER_FONT_H_

#include "../common/ge_include.h"

namespace ge
{


struct GE_API GE_FONT
{
	char	font_key[260];
	int		font_size;
	int		font_charset;
	int		font_weight;
	bool	italic;
	bool	underline;
	bool	strikeout;
};

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

class GE_API GERFontManager
{
protected:
	typedef std::pair<D3DXFONT_DESC, ID3DXFont*>	D3DX_FONT_PAIR;
	typedef std::pair<LOGFONT, HFONT>				GDI_FONT_PAIR;
	typedef std::map<int, D3DX_FONT_PAIR>			D3DX_FONT_MAP;

public:
	GERFontManager();
	virtual ~GERFontManager();

public:
	int			add_font(GE_FONT& font);
	int			add_d3dx_font(GE_FONT& font);
	int			add_gdi_font(GE_FONT& font);
	void		remove_font(int font_id);
	ID3DXFont*	get_d3dx_font(int font_id);
	HFONT		get_gdi_font(int font_id);

protected:
	D3DX_FONT_MAP			font_map_;
	int						auto_font_id_;
};


} // namespace ge

#endif // _GAME_ENGINE_RENDER_TEXT_H_