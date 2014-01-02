#ifndef _CC_TEXTURE_DATA_H_
#define _CC_TEXTURE_DATA_H_

#include "CCData.h"

struct CCContourVetrex
{
	float x;
	float y;
};

class CCContourData
{
public:
	bool			load(Json::Value& json, CCData* data);

public:
	std::vector<CCContourVetrex> m_vertex;
};

class CCTextureData
{
public:
	bool			load(Json::Value& json, CCData* data);

	std::string&	get_name() { return m_name; }
	CCContourData*	get_contour_data(int index = 0);

public:
	std::string					m_name;
	float						m_width;
	float						m_height;
	float						m_pivot_x;
	float						m_pivot_y;
	//std::string				m_plist_file;
	std::vector<CCContourData>	m_contour_data;
};



#endif //_CC_TEXTURE_DATA_H_