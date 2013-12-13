#include "ger_font.h"

namespace ge
{

GERFontManager::GERFontManager( LPDIRECT3DDEVICE9 p_d3d_device )
:p_d3d_device_(p_d3d_device),
auto_font_id_(0)
{

}

GERFontManager::~GERFontManager()
{
}

int GERFontManager::add_font( GE_FONT& font )
{
	if (p_d3d_device_ == NULL) return -1;

	D3DXFONT_DESC dx_font_desc;
	ZeroMemory(&dx_font_desc, sizeof(D3DXFONT_DESC));
	dx_font_desc.Height		= font.font_size;
	dx_font_desc.Weight		= font.font_weight;
	dx_font_desc.CharSet	= font.font_charset;
	dx_font_desc.Italic		= font.italic;
	dx_font_desc.MipLevels	= DEFAULT_QUALITY;
	strcpy_s(dx_font_desc.FaceName, font.font_key);

	ID3DXFont* p_d3d_font = NULL;
	HRESULT h_result = D3DXCreateFontIndirect(p_d3d_device_, &dx_font_desc, &p_d3d_font);
	if (FAILED(h_result)) return -1;
	
	++ auto_font_id_;
	font_map_[auto_font_id_] = std::make_pair(dx_font_desc, p_d3d_font);
	return auto_font_id_;
}

void GERFontManager::remove_font( int font_id )
{
	ID3DXFont* font_obj = get_font(font_id);
	SAFE_RELEASE(font_obj);
	font_map_.erase(font_id);
}

ID3DXFont* GERFontManager::get_font( int font_id )
{
	if (font_map_.find(font_id) == font_map_.end()) return NULL;
	ID3DXFont* font_obj = (ID3DXFont*)font_map_[font_id].second;
	if (font_obj == NULL) return NULL;
	return font_obj;
}



}