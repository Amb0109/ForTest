#include "ger_font.h"
#include "../common/ge_engine.h"

namespace ge
{

GERFontManager::GERFontManager()
:auto_font_id_(0)
{

}

GERFontManager::~GERFontManager()
{
}

GERFontManager* GERFontManager::get_instance()
{
	static GERFontManager _global_p_ge_font_manager;
	return &_global_p_ge_font_manager;
}

int GERFontManager::add_d3dx_font( GE_FONT& font )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if (p_d3d_device == NULL) return -1;

	D3DXFONT_DESC dx_font_desc;
	ZeroMemory(&dx_font_desc, sizeof(D3DXFONT_DESC));
	dx_font_desc.Height		= font.font_size;
	dx_font_desc.Weight		= font.font_weight;
	dx_font_desc.CharSet	= font.font_charset;
	dx_font_desc.Italic		= font.italic;
	dx_font_desc.Quality	= ANTIALIASED_QUALITY;
	strcpy_s(dx_font_desc.FaceName, font.font_key);

	ID3DXFont* p_d3d_font = NULL;
	HRESULT h_res = D3DXCreateFontIndirect(p_d3d_device, &dx_font_desc, &p_d3d_font);
	if (FAILED(h_res)) return -1;
	
	++ auto_font_id_;
	font_map_[auto_font_id_] = std::make_pair(dx_font_desc, p_d3d_font);
	return auto_font_id_;
}

void GERFontManager::remove_font( int font_id )
{
	ID3DXFont* font_obj = get_d3dx_font(font_id);
	SAFE_RELEASE(font_obj);
	font_map_.erase(font_id);
}

ID3DXFont* GERFontManager::get_d3dx_font( int font_id )
{
	if (font_map_.find(font_id) == font_map_.end()) return NULL;
	ID3DXFont* font_obj = (ID3DXFont*)font_map_[font_id].second;
	if (font_obj == NULL) return NULL;
	return font_obj;
}

int GERFontManager::add_gdi_font( GE_FONT& font )
{
	LOGFONT gdi_font_desc;
	ZeroMemory(&gdi_font_desc, sizeof(LOGFONT));
	gdi_font_desc.lfHeight		= font.font_size;
	gdi_font_desc.lfWeight		= font.font_weight;
	gdi_font_desc.lfCharSet		= font.font_charset;
	gdi_font_desc.lfItalic		= font.italic;
	gdi_font_desc.lfStrikeOut	= font.strikeout;
	gdi_font_desc.lfUnderline	= font.underline;
	gdi_font_desc.lfQuality		= ANTIALIASED_QUALITY;
	strcpy_s(gdi_font_desc.lfFaceName, font.font_key);

	HFONT h_font = CreateFontIndirect(&gdi_font_desc);

	// TODO

	return 0;
}



}