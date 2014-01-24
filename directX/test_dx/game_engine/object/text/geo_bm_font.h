#ifndef _GAME_ENGINE_OBJECT_BITMAP_FONT_H_
#define _GAME_ENGINE_OBJECT_BITMAP_FONT_H_

#include "../../common/ge_include.h"
#include <string>

namespace ge
{

struct GEBMFontInfo
{
	int load_with_binary(const char* ptr_data);

	std::string		face_;
	int				size_;
	bool			bold_;
	bool			italic_;
	unsigned		charset_;
	bool			unicode_;
	unsigned		stretchH_;
	bool			smooth_;
	bool			fixedHeigth_;
	unsigned		aa_;
	unsigned		padding_up_;
	unsigned		padding_right_;
	unsigned		padding_down_;
	unsigned		padding_left_;
	unsigned		spacing_h_;
	unsigned		spacing_v_;
	unsigned		outline_;
};

struct GEBMFontCommon
{
	int load_with_binary(const char* ptr_data);

	unsigned		lineHeight_;
	unsigned		base_;
	unsigned		scaleW_;
	unsigned		scaleH_;
	unsigned		pages_;
	bool			packed_;
	unsigned		alphaChnl_;
	unsigned		redChnl_;
	unsigned		greenChnl_;
	unsigned		blueChnl_;
};

struct GEBMFontChar
{
	int load_with_binary(const char* ptr_data);

	unsigned		id_;
	unsigned		x_;
	unsigned		y_;
	unsigned		width_;
	unsigned		height_;
	int				xoffset_;
	int				yoffset_;
	int				xadvance_;
	unsigned		page_;
	unsigned		chnl_;
};

class GE_API GEBMFont
{
public:
	GEBMFont();
	virtual ~GEBMFont();

public:
	bool parse_binary_file(const char* fnt_path);
	bool parse_binary_data(const char* fnt_content, size_t fnt_size);

private:
	typedef std::vector<std::string>						BMF_PAGE_LIST;
	typedef std::map<unsigned, GEBMFontChar>				BMF_CHAR_MAP;
	typedef std::map<std::pair<unsigned, unsigned>, int>	BMF_KERNING_MAP;

	int							version_;
	GEBMFontInfo				info_;
	GEBMFontCommon				common_;
	BMF_PAGE_LIST				pages_;
	BMF_CHAR_MAP				chars_;
	BMF_KERNING_MAP				kerning_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_BITMAP_FONT_H_