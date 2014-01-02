#include "CCData.h"
#include "CCArmatureData.h"
#include "CCAnimationData.h"
#include "CCTextureData.h"
#include "CCAtlasData.h"
#include "support/CCUtils.h"

bool CCBaseData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	x = json.get(A_X, Json::Value(0.f)).asFloat();
	y = json.get(A_Y, Json::Value(0.f)).asFloat();
	zOrder = json.get(A_Z, Json::Value(0)).asInt();
	scaleX = json.get(A_SCALE_X, Json::Value(1.f)).asFloat();
	scaleY = json.get(A_SCALE_Y, Json::Value(1.f)).asFloat();
	skewX = json.get(A_SKEW_X, Json::Value(0.f)).asFloat();
	skewY = json.get(A_SKEW_Y, Json::Value(0.f)).asFloat();

	x *= data->m_position_scale;
	y *= data->m_position_scale;
	x *= data->m_content_scale;
	y *= data->m_content_scale;

	isUseColorInfo = false;
	Json::Value color_info = json.get(COLOR_INFO, Json::Value::jsonNull);

	while (!color_info.isNull())
	{
		if (data->m_version < VERSION_COLOR_READING)
		{
			if (color_info.size() < 1) break;
			color_info = color_info[0];
		}

		int a = color_info.get(A_ALPHA, Json::Value(0xff)).asInt();
		int r = color_info.get(A_RED, Json::Value(0xff)).asInt();
		int g = color_info.get(A_GREEN, Json::Value(0xff)).asInt();
		int b = color_info.get(A_BLUE, Json::Value(0xff)).asInt();
		colorInfo = (a << 24) | (r << 16) | (g << 8) | (b);
		isUseColorInfo = true;
		break;
	}
	return true;
}

void CCBaseData::copy( const CCBaseData* node_data )
{
	if (node_data == NULL) return;
	x = node_data->x;
	y = node_data->y;
	zOrder = node_data->zOrder;

	scaleX = node_data->scaleX;
	scaleY = node_data->scaleY;
	skewX = node_data->skewX;
	skewY = node_data->skewY;

	tweenRotate = node_data->tweenRotate;

	isUseColorInfo = node_data->isUseColorInfo;
	colorInfo = node_data->colorInfo;
}

CCData::CCData()
{
	clear();
}

CCData::~CCData()
{
	clear();
}

bool CCData::load_file( const char* file_path )
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
	return load(pFileContent);
}

bool CCData::load( const char* file_data )
{
	Json::Value json;
	Json::Reader json_reader;
	
	bool ret = true;
	ret = json_reader.parse(std::string(file_data), json, false);
	if (!ret) return false;

	m_content_scale = json.get(CONTENT_SCALE, Json::Value(1.f)).asFloat();

	Json::Value armature_data_json = json.get(ARMATURE_DATA, Json::Value::jsonNull);
	if (armature_data_json.isArray())
	{
		for (unsigned i=0; i<armature_data_json.size(); ++i)
		{
			m_armature_data.push_back(new CCArmatureData());
			if(m_armature_data[i] == NULL) return false;
			if(!m_armature_data[i]->load(armature_data_json[i], this)) return false;
			m_armature_data_map[m_armature_data[i]->get_name()] = i;
		}
	}

	Json::Value animation_data_json = json.get(ANIMATION_DATA, Json::Value::jsonNull);
	if (animation_data_json.isArray())
	{
		for (unsigned i=0; i<animation_data_json.size(); ++i)
		{
			m_animation_data.push_back(new CCAnimationData());
			if(m_animation_data[i] == NULL) return false;
			if(!m_animation_data[i]->load(animation_data_json[i], this)) return false;
			m_animation_data_map[m_animation_data[i]->get_name()] = i;
		}
	}

	Json::Value texture_data_json = json.get(TEXTURE_DATA, Json::Value::jsonNull);
	if (texture_data_json.isArray())
	{
		for (unsigned i=0; i<texture_data_json.size(); ++i)
		{
			m_texture_data.push_back(new CCTextureData());
			if(m_texture_data[i] == NULL) return false;
			if(!m_texture_data[i]->load(texture_data_json[i], this)) return false;
			m_texture_data_map[m_texture_data[i]->get_name()] = i;
		}
	}

	Json::Value config_file_path_json = json.get(CONFIG_FILE_PATH, Json::Value::jsonNull);
	if (config_file_path_json.isArray())
	{
		for (unsigned i=0; i<config_file_path_json.size(); ++i)
		{
			std::string plist_file = config_file_path_json.get(i, Json::Value("")).asString();
			plist_file = m_base_path + plist_file;

			m_atlas_data.push_back(new CCAtlasData());
			if(m_atlas_data[i] == NULL) return false;
			if(!m_atlas_data[i]->load_file(plist_file.c_str())) return false;
		}
	}

	return true;
}

void CCData::clear()
{
	for (unsigned i=0; i<m_armature_data.size(); ++i) delete m_armature_data[i];
	for (unsigned i=0; i<m_animation_data.size(); ++i) delete m_animation_data[i];
	for (unsigned i=0; i<m_texture_data.size(); ++i) delete m_texture_data[i];
	for (unsigned i=0; i<m_atlas_data.size(); ++i) delete m_atlas_data[i];

	m_armature_data.clear();
	m_animation_data.clear();
	m_texture_data.clear();
	m_armature_data.clear();

	m_armature_data_map.clear();
	m_animation_data_map.clear();
	m_texture_data_map.clear();

	m_content_scale = 1.f;
	m_position_scale = 1.f;
	m_version = 0;
	m_base_path.clear();
	m_path.clear();
}

CCArmatureData* CCData::get_armature_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_armature_data.size() <= (unsigned)index) return NULL;
	return m_armature_data[(unsigned)index];
}

CCArmatureData* CCData::get_armature_data_by_name( const char* name )
{
	if (m_armature_data_map.find(name) == m_armature_data_map.end()) return NULL;
	return get_armature_data(m_armature_data_map[name]);
}

CCAnimationData* CCData::get_animation_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_animation_data.size() <= (unsigned)index) return NULL;
	return m_animation_data[(unsigned)index];
}

CCAnimationData* CCData::get_animation_data_by_name( const char* name )
{
	if (m_animation_data_map.find(name) == m_animation_data_map.end()) return NULL;
	return get_animation_data(m_animation_data_map[name]);
}

CCTextureData* CCData::get_texture_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_texture_data.size() <= (unsigned)index) return NULL;
	return m_texture_data[(unsigned)index];
}

CCTextureData* CCData::get_texture_data_by_name( const char* name )
{
	if (m_texture_data_map.find(name) == m_texture_data_map.end()) return NULL;
	return get_texture_data(m_texture_data_map[name]);
}

CCAtlasData* CCData::get_atlas_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_atlas_data.size() <= (unsigned)index) return NULL;
	return m_atlas_data[(unsigned)index];
}



