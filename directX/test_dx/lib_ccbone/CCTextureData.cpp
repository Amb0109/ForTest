#include "CCTextureData.h"

bool CCTextureData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	m_name = json.get(A_NAME, Json::Value("")).asString();
	m_width = json.get(A_WIDTH, Json::Value(0.f)).asFloat();
	m_height = json.get(A_HEIGHT, Json::Value(0.f)).asFloat();
	m_pivot_x = json.get(A_PIVOT_X, Json::Value(0.f)).asFloat();
	m_pivot_y = json.get(A_PIVOT_Y, Json::Value(0.f)).asFloat();
	//m_plistFile = json.get("plistFile", Json::Value("")).asString();

	Json::Value contour_data_json = json.get(CONTOUR_DATA, Json::Value::jsonNull);
	if (contour_data_json.isArray())
	{
		for (unsigned i=0; i<contour_data_json.size(); ++i)
		{
			CCContourData contour_data;
			if(!contour_data.load(contour_data_json[i], data)) return false;
			m_contour_data.push_back(contour_data);
		}
	}

	return true;
}

CCContourData* CCTextureData::get_contour_data( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_contour_data.size() <= (unsigned)index) return NULL;
	return &(m_contour_data[(unsigned)index]);
}

bool CCContourData::load( Json::Value& json, CCData* data )
{
	if(json == Json::Value::jsonNull) return false;

	Json::Value contour_vertex_json = json.get(VERTEX_POINT, Json::Value::jsonNull);
	if (contour_vertex_json.isArray())
	{
		for (unsigned i=0; i<contour_vertex_json.size(); ++i)
		{
			CCContourVetrex contour_vertex;
			contour_vertex.x = contour_vertex_json[i].get(A_X, Json::Value(0.f)).asFloat();
			contour_vertex.y = contour_vertex_json[i].get(A_Y, Json::Value(0.f)).asFloat();
			m_vertex.push_back(contour_vertex);
		}
	}
	return true;
}
