#ifndef _GAME_ENGINE_OBJECT_BITMAP_FONT_H_
#define _GAME_ENGINE_OBJECT_BITMAP_FONT_H_

#include "../../common/ge_include.h"
#include "../../object/text/geo_text_bm.h"
#include "bm_font/bm_font.h"
#include "ge_font.h"

namespace ge
{

class GETexture;
class GE_API GEFontBM : public GEFont
{
	DLL_MANAGE_CLASS(GEFontBM);

public:
	GEFontBM();
	virtual ~GEFontBM();

public:
	virtual bool init(const char* fnt_path);

	int  get_page_cnt();
	bool get_page_path(char* texture_path, int index);

	bool begin_write(bmfont::SCharRenderObject* char_buff, int buff_size);
	int end_write();

	bool write_text(const char* text, int width, int height, bool wrap);
	
private:
	bmfont::CFont		bm_font_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_BITMAP_FONT_H_