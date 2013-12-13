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
};

struct GE_API GE_TEXT_STYLE
{
	int			font_id;
	int			format;
	D3DCOLOR	font_color;
	bool		underline;
	bool		strikeout;
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
	typedef std::pair<D3DXFONT_DESC, ID3DXFont*>	FONT_PAIR;
	typedef std::map<int, FONT_PAIR>		FONT_MAP;

public:
	GERFontManager(LPDIRECT3DDEVICE9 p_d3d_device);
	virtual ~GERFontManager();

public:
	int			add_font(GE_FONT& font);
	void		remove_font(int font_id);
	ID3DXFont*	get_font(int font_id);

protected:
	LPDIRECT3DDEVICE9	p_d3d_device_;
	FONT_MAP			font_map_;
	int					auto_font_id_;
};


} // namespace ge

#endif // _GAME_ENGINE_RENDER_TEXT_H_