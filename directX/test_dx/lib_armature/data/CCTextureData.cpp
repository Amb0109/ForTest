#include "CCTextureData.h"

#include "CCDataDef.h"

namespace CC {

bool CCTextureData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	m_pDataCollection = data;

	m_sName = json.get(A_NAME, Json::Value("")).asString();
	m_fWidth = json.get(A_WIDTH, Json::Value(0.f)).asFloat();
	m_fHeight = json.get(A_HEIGHT, Json::Value(0.f)).asFloat();
	m_fPivotX = json.get(A_PIVOT_X, Json::Value(0.f)).asFloat();
	m_fPivotY = json.get(A_PIVOT_Y, Json::Value(0.f)).asFloat();
	//m_sPlistFile = json.get("plistFile", Json::Value("")).asString();

	Json::Value contourDataJson = json.get(CONTOUR_DATA, Json::Value::null);
	if (contourDataJson.isArray())
	{
		for (unsigned i=0; i<contourDataJson.size(); ++i)
		{
			CCContourData contourData;
			if(!contourData.load(contourDataJson[i], data)) return false;
			m_vecContourData.push_back(contourData);
		}
	}

	return true;
}

CCContourData* CCTextureData::getContourData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecContourData.size() <= (unsigned)index) return NULL;
	return &(m_vecContourData[(unsigned)index]);
}

bool CCContourData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	Json::Value contourCertexJson = json.get(VERTEX_POINT, Json::Value::null);
	if (contourCertexJson.isArray())
	{
		for (unsigned i=0; i<contourCertexJson.size(); ++i)
		{
			CCContourVetrex contourVertex;
			contourVertex.x = contourCertexJson[i].get(A_X, Json::Value(0.f)).asFloat();
			contourVertex.y = contourCertexJson[i].get(A_Y, Json::Value(0.f)).asFloat();
			m_vecVertiesList.push_back(contourVertex);
		}
	}
	return true;
}

} // namespace