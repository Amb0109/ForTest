#ifndef _CC_ATLAS_DATA_H_
#define _CC_ATLAS_DATA_H_

#include "support/CCSAXParser.h"
#include <stack>
#include <map>
#include <string>

class CCAtlasMaker;
class CCAtlasData;
struct CCAtlasFrame;

typedef enum 
{
	TYPE_NONE = 0,
	TYPE_KEY,
	TYPE_DICT,
	TYPE_INT,
	TYPE_REAL,
	TYPE_STRING,
	TYPE_ARRAY
} CCSaxType;

typedef enum
{
	DICT_NONE = 0,
	DICT_MAIN,
	DICT_FRAMES,
	DICT_FRAME,
	DICT_METADATA,
	DICT_TEXTURE
} CCAtlasDict;

class CCAtlasMaker : public CCSAXDelegator
{
public:
	static CCAtlasMaker* get_instance() { static CCAtlasMaker _global_atlas_maker; return &_global_atlas_maker;}

public:
	void startElement(void *ctx, const char* name, const char** atts);
	void endElement(void *ctx, const char* name);
	void textHandler(void *ctx, const char *ch, int len);

public:
	void set_atlas(CCAtlasData* atlas) { m_atlas = atlas; }
	bool prase(const char* xml_data, int len);
	void clear();

private:
	CCAtlasData*						m_atlas;
	CCAtlasDict						m_cur_dict;
	std::string						m_cur_dict_key;
	std::stack<CCSaxType>			m_type_stack;
	std::stack<std::string>			m_key_stack;
};

class CCAtlasData
{
public:
	typedef std::map<std::string, CCAtlasFrame> ATLAS_FRAME_MAP;

public:
	bool load_file(const char* file_path);
	bool load(const char* file_data, int len);
	
	void clear();

	CCAtlasFrame*			get_frame(const char* name);

public:
	ATLAS_FRAME_MAP			m_frames;

	int						m_width;
	int						m_height;
	std::string				m_base_path;
	std::string				m_path;
	std::string				m_img_file;
};

struct CCAtlasFrame
{
	std::string		m_name;
	int				m_width;
	int				m_height;
	int				m_original_width;
	int				m_original_height;
	int				m_x;
	int				m_y;
	float			m_offset_x;
	float			m_offset_y;
};

#endif // _CC_ATLAS_DATA_H_