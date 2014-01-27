#include "ge_bm_font.h"
#include "geo_text_bm.h"

namespace ge
{

const int BMF_CHAR_SIZE = 20;
const int BMF_KERNING_SIZE = 10;

inline int _bmfont_get_int( char** ptr_fnt, int byte_cnt )
{
	int ret = 0;
	switch (byte_cnt)
	{
	case 1:
		ret = (*(signed char*)(*ptr_fnt));
		break;
	case 2:
		ret = (*(signed short*)(*ptr_fnt));
		break;
	case 4:
		ret = (*(signed int*)(*ptr_fnt));
		break;
	default:
		break;
	}

	(*ptr_fnt) += byte_cnt;
	return ret;
}

inline unsigned _bmfont_get_uint( char** ptr_fnt, int byte_cnt )
{
	int ret = 0;
	switch (byte_cnt)
	{
	case 1:
		ret = (*(unsigned char*)(*ptr_fnt));
		break;
	case 2:
		ret = (*(unsigned short*)(*ptr_fnt));
		break;
	case 4:
		ret = (*(unsigned int*)(*ptr_fnt));
		break;
	default:
		break;
	}

	(*ptr_fnt) += byte_cnt;
	return ret;
}

inline const char* _bmfont_get_string( char** ptr_fnt )
{
	char* ret = *ptr_fnt;
	(*ptr_fnt) += strlen(ret) + 1;
	return ret;
}

int GEBMFontInfo::load_with_binary( const char* ptr_data )
{
	char* ptr_fnt = (char*)ptr_data;
	if (ptr_fnt == NULL) return -1;

	size_ = _bmfont_get_int(&ptr_fnt, 2);

	unsigned bit_field = 0;
	bit_field		= _bmfont_get_uint(&ptr_fnt, 1);
	smooth_			= 0 != (bit_field & 0x80);
	unicode_		= 0 != (bit_field & 0x40);
	italic_			= 0 != (bit_field & 0x20);
	bold_			= 0 != (bit_field & 0x10);
	fixedHeigth_	= 0 != (bit_field & 0x10);

	charset_		= _bmfont_get_uint(&ptr_fnt, 1);
	stretchH_		= _bmfont_get_uint(&ptr_fnt, 2);
	aa_				= _bmfont_get_uint(&ptr_fnt, 1);
	padding_up_		= _bmfont_get_uint(&ptr_fnt, 1);
	padding_right_	= _bmfont_get_uint(&ptr_fnt, 1);
	padding_down_	= _bmfont_get_uint(&ptr_fnt, 1);
	padding_left_	= _bmfont_get_uint(&ptr_fnt, 1);
	spacing_h_		= _bmfont_get_uint(&ptr_fnt, 1);
	spacing_v_		= _bmfont_get_uint(&ptr_fnt, 1);
	outline_		= _bmfont_get_uint(&ptr_fnt, 1);
	face_			= _bmfont_get_string(&ptr_fnt);

	return int(ptr_fnt - ptr_data);
}

int GEBMFontCommon::load_with_binary( const char* ptr_data )
{
	char* ptr_fnt = (char*)ptr_data;
	if (ptr_fnt == NULL) return -1;

	lineHeight_	= _bmfont_get_uint(&ptr_fnt, 2);
	base_		= _bmfont_get_uint(&ptr_fnt, 2);
	scaleW_		= _bmfont_get_uint(&ptr_fnt, 2);
	scaleH_		= _bmfont_get_uint(&ptr_fnt, 2);
	pages_		= _bmfont_get_uint(&ptr_fnt, 2);
	
	unsigned bit_field = 0;
	bit_field	= _bmfont_get_uint(&ptr_fnt, 1);
	packed_		= 0 != (bit_field & 0x1);

	alphaChnl_	= _bmfont_get_uint(&ptr_fnt, 1);
	redChnl_	= _bmfont_get_uint(&ptr_fnt, 1);
	greenChnl_	= _bmfont_get_uint(&ptr_fnt, 1);
	blueChnl_	= _bmfont_get_uint(&ptr_fnt, 1);

	return int(ptr_fnt - ptr_data);
}


int GEBMFontChar::load_with_binary( const char* ptr_data )
{
	char* ptr_fnt = (char*)ptr_data;
	if (ptr_fnt == NULL) return -1;

	id_			= _bmfont_get_uint(&ptr_fnt, 4);
	x_			= _bmfont_get_uint(&ptr_fnt, 2);
	y_			= _bmfont_get_uint(&ptr_fnt, 2);
	width_		= _bmfont_get_uint(&ptr_fnt, 2);
	height_		= _bmfont_get_uint(&ptr_fnt, 2);
	xoffset_	= _bmfont_get_int(&ptr_fnt, 2);
	yoffset_	= _bmfont_get_int(&ptr_fnt, 2);
	xadvance_	= _bmfont_get_int(&ptr_fnt, 2);
	page_		= _bmfont_get_uint(&ptr_fnt, 1);
	chnl_		= _bmfont_get_uint(&ptr_fnt, 1);

	return int(ptr_fnt - ptr_data);
}


GEBMFont::GEBMFont()
{

}

GEBMFont::~GEBMFont()
{

}

bool GEBMFont::parse_binary_file( const char* fnt_path )
{
	FILE* fp = fopen(fnt_path, "rb");
	if (fp == NULL) return false;

	fseek(fp,0,SEEK_END);
	size_t size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	if (size <= 0) { fclose(fp); return false; }
	
	char* buff = NULL;
	buff = new char[size];
	size = fread(buff, sizeof(char), size, fp);
	fclose(fp);
	
	std::string work_dir = fnt_path;
	size_t find_pos = work_dir.rfind("\\");
	if (find_pos != work_dir.npos)
	{
		work_dir.erase(find_pos);
		work_dir.append("\\");
	} else work_dir = ".\\";

	bool ret = parse_binary_data(buff, size);
	delete buff;

	png_dir_ = work_dir;

	return ret;
}

bool GEBMFont::parse_binary_data( const char* fnt_content, size_t fnt_size )
{
	if (fnt_content == NULL) return false;
	if (fnt_size <= 0) return false;
	png_dir_.clear();

	char* ptr_fnt = (char*)fnt_content;
	if(		*(ptr_fnt++) != 'B'
		||	*(ptr_fnt++) != 'M'
		||	*(ptr_fnt++) != 'F')
	{
		return false;
	}

	version_ = _bmfont_get_int(&ptr_fnt, 1);
	assert(version_ == 3);

	int block_type = 0;
	int block_size = 0;
	int ret_size = 0;

	while((size_t)(ptr_fnt - fnt_content) < fnt_size)
	{
		block_type = _bmfont_get_uint(&ptr_fnt, 1);
		switch (block_type)
		{
		case 1: // info block
			{
				block_size = _bmfont_get_uint(&ptr_fnt, 4);
				ret_size = info_.load_with_binary(ptr_fnt);
				assert(block_size == ret_size);
				ptr_fnt += block_size;
			}
			break;
		case 2: // common block
			{
				block_size = _bmfont_get_uint(&ptr_fnt, 4);
				ret_size = common_.load_with_binary(ptr_fnt);
				assert(block_size == ret_size);
				ptr_fnt += block_size;
			}
			break;
		case 3: // page block
			{
				block_size = _bmfont_get_uint(&ptr_fnt, 4);
				pages_.clear();
				for (unsigned i=0; i<common_.pages_; ++i)
				{
					pages_.push_back(_bmfont_get_string(&ptr_fnt));
				}
			}
			break;
		case 4: // char block
			{
				block_size = _bmfont_get_uint(&ptr_fnt, 4);
				unsigned char_cnt = block_size / BMF_CHAR_SIZE;
				assert(BMF_CHAR_SIZE * char_cnt == block_size);
				chars_.clear();
				for (unsigned i=0; i<char_cnt; ++i)
				{
					GEBMFontChar font_char;
					int ret_size = font_char.load_with_binary(ptr_fnt);
					assert(ret_size == BMF_CHAR_SIZE);
					chars_[font_char.id_] = font_char;
					ptr_fnt += BMF_CHAR_SIZE;
				}
			}
			break;
		case 5: // kerning block
			{
				block_size = _bmfont_get_uint(&ptr_fnt, 4);
				int kerning_cnt = block_size / BMF_KERNING_SIZE;
				assert(BMF_KERNING_SIZE * kerning_cnt == block_size);
				kerning_.clear();
				for (int i=0; i<kerning_cnt; ++i)
				{
					unsigned first = _bmfont_get_uint(&ptr_fnt, 4);
					unsigned second = _bmfont_get_uint(&ptr_fnt, 4);
					int amount = _bmfont_get_int(&ptr_fnt, 2);
					kerning_[std::make_pair(first, second)] = amount;
				}
			}
			break;
		default:
			{
				++ptr_fnt;
			}
		}
	}

	return true;
}

void GEBMFont::set_png_dir( const char* png_dir )
{
	png_dir_ = png_dir;
}

int GEBMFont::get_png_cnt()
{
	return (int)common_.pages_;
}

void GEBMFont::get_png_path( char* out_png_path, int index )
{
	if (index < 0 || index >= (int)png_dir_.size()) return;
	std::string png_path = png_dir_ + pages_[index];
	strcpy(out_png_path, png_path.c_str());
}

bool GEBMFont::compose( GEOTextBM* out_text, const char* text, int width, int height, bool wrap )
{
	if (out_text == NULL) return false;
	out_text->clear_render_chars();

	if (text == NULL) return true;

	int pen_x = 0;
	int pen_y = 0;

	for (int i=0; text[i]; ++i)
	{
		switch (text[i])
		{
		case '\r':
		case '\n':
			{
				pen_x = 0;
				pen_y += common_.lineHeight_;
			}
			break;
		default:
			if (chars_.find(text[i]) != chars_.end())
			{
				GEBMFontChar& bm_char = chars_[text[i]];
				GE_TEXT_CHAR text_char;
				text_char.ch_			= text[i];
				text_char.visible_		= true;
				text_char.pos_.x		= pen_x + bm_char.xoffset_;
				text_char.pos_.y		= pen_y + bm_char.yoffset_;
				text_char.size_.width	= bm_char.width_;
				text_char.size_.height	= bm_char.height_;
				text_char.img_			= bm_char.page_;
				text_char.img_pos_.x	= bm_char.x_;
				text_char.img_pos_.y	= bm_char.y_;
				out_text->add_render_char(text_char);

				pen_x += bm_char.xadvance_;
			}
		}
	}
	return true;
}





}

