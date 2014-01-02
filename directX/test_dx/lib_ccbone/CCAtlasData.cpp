#include "CCAtlasData.h"
#include "support/CCUtils.h"
#include "support/tinyxml2/tinyxml2.h"
#include "assert.h"

void CCAtlasMaker::startElement( void *ctx, const char* name, const char** atts )
{
	if (strcmp(name, "dict") == 0)
	{
		m_type_stack.push(TYPE_DICT);
		switch (m_key_stack.size())
		{
		case 0:
			{
				m_cur_dict = DICT_MAIN;
			}
			break;
		case 1:
			{
				const char* dict_key = m_key_stack.top().c_str();
				if (strcmp(dict_key, "frames") == 0)
				{
					m_cur_dict = DICT_FRAMES;
				}
				else if (strcmp(dict_key, "metadata") == 0)
				{
					m_cur_dict = DICT_METADATA;
				}
				else if (strcmp(dict_key, "texture") == 0)
				{
					m_cur_dict = DICT_TEXTURE;
				}
				else
				{
					m_cur_dict = DICT_NONE;
				}
			}
			break;
		case 2:
			if (m_cur_dict == DICT_FRAMES)
			{
				m_cur_dict = DICT_FRAME;
			}
			break;
		default:
			m_cur_dict = DICT_NONE;
		}

		if (!m_key_stack.empty()) m_cur_dict_key = m_key_stack.top();
		else m_cur_dict_key = "";

		m_key_stack.push("");
	}
	else if (strcmp(name, "key") == 0)
	{
		m_type_stack.push(TYPE_KEY);
	}
	else if (strcmp(name, "integer") == 0)
	{
		m_type_stack.push(TYPE_INT);
	}
	else if (strcmp(name, "real") == 0)
	{
		m_type_stack.push(TYPE_REAL);
	}
	else if (strcmp(name, "string") == 0)
	{
		m_type_stack.push(TYPE_STRING);
	}
	else if (strcmp(name, "array") == 0)
	{
		m_type_stack.push(TYPE_ARRAY);
	}
	else
	{
		//assert(false);
	}
}

void CCAtlasMaker::endElement( void *ctx, const char* name )
{
	if (strcmp(name, "dict") == 0)
	{
		if(!m_type_stack.empty() && TYPE_DICT == m_type_stack.top())
		{
			m_type_stack.pop();
			m_key_stack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "key") == 0)
	{
		if(!m_type_stack.empty() && TYPE_KEY == m_type_stack.top())
		{
			m_type_stack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "integer") == 0)
	{
		if(!m_type_stack.empty() && TYPE_INT == m_type_stack.top())
		{
			m_type_stack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "real") == 0)
	{
		if(!m_type_stack.empty() && TYPE_REAL == m_type_stack.top())
		{
			m_type_stack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "string") == 0)
	{
		if(!m_type_stack.empty() && TYPE_STRING == m_type_stack.top())
		{
			m_type_stack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "array") == 0)
	{
		if(!m_type_stack.empty() && TYPE_ARRAY == m_type_stack.top())
		{
			m_type_stack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else
	{
		//assert(false);
	}
}

void CCAtlasMaker::textHandler( void *ctx, const char *ch, int len )
{
	if (m_type_stack.empty()) return;
	if (m_atlas == NULL) return;

	if (TYPE_KEY == m_type_stack.top())
	{
		m_key_stack.pop();
		m_key_stack.push(ch);
		return;
	}

	const char* cur_key = m_key_stack.top().c_str();
	switch (m_cur_dict)
	{
	case DICT_FRAME:
		{
			if (m_atlas->m_frames.find(m_cur_dict_key) == m_atlas->m_frames.end())
			{
				m_atlas->m_frames[m_cur_dict_key].m_name = m_cur_dict_key;
			}

			CCAtlasFrame& frame = m_atlas->m_frames[m_cur_dict_key];
			if (strcmp(cur_key, "width") == 0) tinyxml2::XMLUtil::ToInt(ch, &(frame.m_width));
			else if (strcmp(cur_key, "height") == 0) tinyxml2::XMLUtil::ToInt(ch, &(frame.m_height));
			else if (strcmp(cur_key, "x") == 0) tinyxml2::XMLUtil::ToInt(ch, &(frame.m_x));
			else if (strcmp(cur_key, "y") == 0) tinyxml2::XMLUtil::ToInt(ch, &(frame.m_y));
			else if (strcmp(cur_key, "originalWidth") == 0) tinyxml2::XMLUtil::ToInt(ch, &(frame.m_original_width));
			else if (strcmp(cur_key, "originalHeight") == 0) tinyxml2::XMLUtil::ToInt(ch, &(frame.m_original_height));
			else if (strcmp(cur_key, "offsetX") == 0) tinyxml2::XMLUtil::ToFloat(ch, &(frame.m_offset_x));
			else if (strcmp(cur_key, "offsetY") == 0) tinyxml2::XMLUtil::ToFloat(ch, &(frame.m_offset_y));
		}
		break;
	case DICT_METADATA:
		{
			if (strcmp(cur_key, "textureFileName") == 0) m_atlas->m_img_file = m_atlas->m_base_path + ch;
		}
		break;
	case DICT_TEXTURE:
		{
			if (strcmp(cur_key, "width") == 0) tinyxml2::XMLUtil::ToInt(ch, &(m_atlas->m_width));
			else if (strcmp(cur_key, "height") == 0) tinyxml2::XMLUtil::ToInt(ch, &(m_atlas->m_height));
		}
		break;
	default:
		break;
	}
}

bool CCAtlasMaker::prase( const char* xml_data, int len )
{
	CCSAXParser parser;
	parser.setDelegator(this);
	if(!parser.init("UTF-8")) return false;
	return parser.parse(xml_data, len);
}

void CCAtlasMaker::clear()
{
	m_atlas = NULL;
	m_cur_dict = DICT_NONE;
	m_cur_dict_key.clear();
	while(!m_type_stack.empty()) m_type_stack.pop();
	while(!m_key_stack.empty()) m_key_stack.pop();
}

bool CCAtlasData::load_file( const char* file_path )
{
	//! find the base file path
	m_base_path = file_path;
	m_path = file_path;
	size_t pos = m_base_path.find_last_of("/");
	if (pos != std::string::npos)
	{
		m_base_path = m_base_path.substr(0, pos + 1);
	}
	else
	{
		m_base_path = "";
	}

	std::string filePathStr =  file_path;
	size_t startPos = filePathStr.find_last_of(".");
	std::string str = &filePathStr[startPos];

	unsigned long size;
	const char *pFileContent = (const char*)getFileData(file_path , "r", &size);
	if (pFileContent == NULL || size <= 0) return false;
	return load(pFileContent, size);
}

bool CCAtlasData::load( const char* file_data, int len)
{
	CCAtlasMaker* atlas_maker = CCAtlasMaker::get_instance();
	atlas_maker->clear();
	atlas_maker->set_atlas(this);
	return atlas_maker->prase(file_data, len);
}

void CCAtlasData::clear()
{
	m_frames.clear();
	m_width = 0;
	m_height = 0;
	m_base_path.clear();
	m_img_file.clear();
}

CCAtlasFrame* CCAtlasData::get_frame( const char* name )
{
	if (m_frames.find(name) == m_frames.end()) return NULL;
	return &(m_frames[name]);
}
