#include "CCData.h"
#include "CCArmatureData.h"
#include "CCAnimationData.h"
#include "CCTextureData.h"
#include "CCAtlasData.h"
#include "../cocos/CCUtils.h"
#include <algorithm>
#include <cmath>

#include "CCDataDef.h"

namespace CC {

CCBaseData::CCBaseData()
: x(0.0f)
, y(0.0f)
, zOrder(0)
, skewX(0.0f)
, skewY(0.0f)
, scaleX(1.0f)
, scaleY(1.0f)
, tweenRotate(0.0f)
, isUseColorInfo(false)
, a(0xff)
, r(0xff)
, g(0xff)
, b(0xff)
{
}


bool CCBaseData::load( Json::Value& json, CCDataCollection* data )
{
	if(json == Json::Value::null) return false;

	x = json.get(A_X, Json::Value(0.f)).asFloat();
	y = json.get(A_Y, Json::Value(0.f)).asFloat();
	zOrder = json.get(A_Z, Json::Value(0)).asInt();
	scaleX = json.get(A_SCALE_X, Json::Value(1.f)).asFloat();
	scaleY = json.get(A_SCALE_Y, Json::Value(1.f)).asFloat();
	skewX = json.get(A_SKEW_X, Json::Value(0.f)).asFloat();
	skewY = json.get(A_SKEW_Y, Json::Value(0.f)).asFloat();

	x *= data->m_sPositionScale;
	y *= data->m_sPositionScale;
	x *= data->m_fContentScale;
	y *= data->m_fContentScale;

	isUseColorInfo = false;
	Json::Value color_info = json.get(COLOR_INFO, Json::Value::null);

	while (!color_info.isNull())
	{
		if (data->m_fVersion < VERSION_COLOR_READING)
		{
			if (color_info.size() < 1) break;
			color_info = color_info[0];
		}

		a = color_info.get(A_ALPHA, Json::Value(0xff)).asInt();
		r = color_info.get(A_RED, Json::Value(0xff)).asInt();
		g = color_info.get(A_GREEN, Json::Value(0xff)).asInt();
		b = color_info.get(A_BLUE, Json::Value(0xff)).asInt();
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
	a = node_data->a;
	r = node_data->r;
	g = node_data->g;
	b = node_data->b;
}

void CCBaseData::subtract( CCBaseData *from, CCBaseData *to, bool limit )
{
	x = to->x - from->x;
	y = to->y - from->y;
	scaleX = to->scaleX - from->scaleX;
	scaleY = to->scaleY - from->scaleY;
	skewX = to->skewX - from->skewX;
	skewY = to->skewY - from->skewY;

	if(isUseColorInfo || from->isUseColorInfo || to->isUseColorInfo)
	{
		a = to->a - from->a;
		r = to->r - from->r;
		g = to->g - from->g;
		b = to->b - from->b;

		isUseColorInfo = true;
	}
	else
	{
		a = r = g = b = 0;
		isUseColorInfo = false;
	}

	if (limit)
	{
		if (skewX > M_PI)
		{
			skewX -= (float)(M_PI * 2.f);
		}
		if (skewX < -M_PI)
		{
			skewX += (float)(M_PI * 2.f);
		}

		if (skewY > M_PI)
		{
			skewY -= (float)(M_PI * 2.f);
		}
		if (skewY < -M_PI)
		{
			skewY += (float)(M_PI * 2.f);
		}
	}


	if (to->tweenRotate)
	{
		skewX += to->tweenRotate * M_PI * 2;
		skewY -= to->tweenRotate * M_PI * 2;
	}
}

CCDataCollection::CCDataCollection()
{
	release();
}

CCDataCollection::~CCDataCollection()
{
	release();
}

bool CCDataCollection::loadFile( const char* filePath )
{
	//! find the base file path
	m_sBasePath = filePath;
	std::transform(m_sBasePath.begin(), m_sBasePath.end(), m_sBasePath.begin(), convertSplash);

	m_sPath = m_sBasePath;
	size_t pos = m_sBasePath.find_last_of("/");
	if (pos != std::string::npos)
	{
		m_sBasePath = m_sBasePath.substr(0, pos + 1);
	}
	else
	{
		m_sBasePath = "";
	}

	std::string filePathStr =  m_sPath;
	size_t startPos = filePathStr.find_last_of(".");
	std::string str = &filePathStr[startPos];

	unsigned long size;
	char* pFileContent = (char*)getFileBuf(m_sPath.c_str() , "r", &size);
	if (pFileContent == NULL || size <= 0) return false;
	bool ret = _load(pFileContent);
	delete pFileContent;
	return ret;
}

bool CCDataCollection::_load( const char* file_data )
{
	Json::Value json;
	Json::Reader jsonReader;
	
	bool ret = true;
	ret = jsonReader.parse(std::string(file_data), json, false);
	if (!ret) return false;

	m_fContentScale = json.get(CONTENT_SCALE, Json::Value(1.f)).asFloat();

	Json::Value armatureDataJson = json.get(ARMATURE_DATA, Json::Value::null);
	if (armatureDataJson.isArray())
	{
		for (unsigned i=0; i<armatureDataJson.size(); ++i)
		{
			m_vecArmatureDataList.push_back(new CCArmatureData());
			if(m_vecArmatureDataList[i] == NULL) return false;
			if(!m_vecArmatureDataList[i]->load(armatureDataJson[i], this)) return false;
			m_mapArmatureDataMap[m_vecArmatureDataList[i]->getName()] = i;
		}
	}

	Json::Value animationDataJson = json.get(ANIMATION_DATA, Json::Value::null);
	if (animationDataJson.isArray())
	{
		for (unsigned i=0; i<animationDataJson.size(); ++i)
		{
			m_vecAnimationDataList.push_back(new CCAnimationData());
			if(m_vecAnimationDataList[i] == NULL) return false;
			if(!m_vecAnimationDataList[i]->load(animationDataJson[i], this)) return false;
			m_mapAnimationDataMap[m_vecAnimationDataList[i]->getName()] = i;
		}
	}

	Json::Value textureDataJson = json.get(TEXTURE_DATA, Json::Value::null);
	if (textureDataJson.isArray())
	{
		for (unsigned i=0; i<textureDataJson.size(); ++i)
		{
			m_vecTextureDataList.push_back(new CCTextureData());
			if(m_vecTextureDataList[i] == NULL) return false;
			if(!m_vecTextureDataList[i]->load(textureDataJson[i], this)) return false;
			m_mapTextureDataMap[m_vecTextureDataList[i]->getName()] = i;
		}
	}

	Json::Value configFilePathJson = json.get(CONFIG_FILE_PATH, Json::Value::null);
	if (configFilePathJson.isArray())
	{
		for (unsigned i=0; i<configFilePathJson.size(); ++i)
		{
			std::string sPlistFile = configFilePathJson.get(i, Json::Value("")).asString();
			sPlistFile = m_sBasePath + sPlistFile;

			m_vecAtlasPagesList.push_back(new CCAtlasPage());
			if(m_vecAtlasPagesList[i] == NULL) return false;
			if(!m_vecAtlasPagesList[i]->loadFile(sPlistFile.c_str())) return false;
		}
	}

	return true;
}

void CCDataCollection::release()
{
	for (unsigned i=0; i<m_vecArmatureDataList.size(); ++i) delete m_vecArmatureDataList[i];
	for (unsigned i=0; i<m_vecAnimationDataList.size(); ++i) delete m_vecAnimationDataList[i];
	for (unsigned i=0; i<m_vecTextureDataList.size(); ++i) delete m_vecTextureDataList[i];
	for (unsigned i=0; i<m_vecAtlasPagesList.size(); ++i) delete m_vecAtlasPagesList[i];

	m_vecArmatureDataList.clear();
	m_vecAnimationDataList.clear();
	m_vecTextureDataList.clear();
	m_vecArmatureDataList.clear();

	m_mapArmatureDataMap.clear();
	m_mapAnimationDataMap.clear();
	m_mapTextureDataMap.clear();

	m_fContentScale = 1.f;
	m_sPositionScale = 1.f;
	m_fVersion = 0;
	m_sBasePath.clear();
	m_sPath.clear();
}

CCArmatureData* CCDataCollection::getArmatureData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecArmatureDataList.size() <= (unsigned)index) return NULL;
	return m_vecArmatureDataList[(unsigned)index];
}

CCArmatureData* CCDataCollection::getArmatureDataByName( const char* name )
{
	if (m_mapArmatureDataMap.find(name) == m_mapArmatureDataMap.end()) return NULL;
	return getArmatureData(m_mapArmatureDataMap[name]);
}

CCAnimationData* CCDataCollection::getAnimationData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecAnimationDataList.size() <= (unsigned)index) return NULL;
	return m_vecAnimationDataList[(unsigned)index];
}

CCAnimationData* CCDataCollection::getAnimationDataByName( const char* name )
{
	if (m_mapAnimationDataMap.find(name) == m_mapAnimationDataMap.end()) return NULL;
	return getAnimationData(m_mapAnimationDataMap[name]);
}

CCTextureData* CCDataCollection::getTextureData( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecTextureDataList.size() <= (unsigned)index) return NULL;
	return m_vecTextureDataList[(unsigned)index];
}

CCTextureData* CCDataCollection::getTextureDataByName( const char* name )
{
	if (m_mapTextureDataMap.find(name) == m_mapTextureDataMap.end()) return NULL;
	return getTextureData(m_mapTextureDataMap[name]);
}

CCAtlasPage* CCDataCollection::getAtlasPage( int index /*= 0*/ )
{
	if (index < 0) return NULL;
	if (m_vecAtlasPagesList.size() <= (unsigned)index) return NULL;
	return m_vecAtlasPagesList[(unsigned)index];
}

CCAtlasData* CCDataCollection::getAtlasData( const char* atlasName )
{
	for (int i=0; ; ++i)
	{
		CCAtlasPage* pAtlasPage = getAtlasPage(i);
		if (pAtlasPage == NULL) break;
		CCAtlasData* pAtlasData = NULL;
		pAtlasData = pAtlasPage->getAtlasData(atlasName);
		if(pAtlasData == NULL) continue;
		return pAtlasData;
	}
	return NULL;
}


CCDataManager::CCDataManager()
: m_mapDatas()
{
}

CCDataManager::~CCDataManager()
{
}

CCDataManager* CCDataManager::getInstance()
{
	static CCDataManager _globalCCDataManager;
	return &_globalCCDataManager;
}

CCDataCollection* CCDataManager::getData( const char* dataFile )
{
	if (m_mapDatas.find(dataFile) != m_mapDatas.end())
	{
		CCDataCollection* pDataCollection = m_mapDatas[dataFile];
		++ m_mapRefs[pDataCollection];
		return pDataCollection;
	}

	CCDataCollection* pDataCollection = new CCDataCollection();
	if(NULL == pDataCollection) return NULL;

	pDataCollection->loadFile(dataFile);
	m_mapDatas[dataFile] = pDataCollection;
	m_mapRefs[pDataCollection] = 1;
	return pDataCollection;
}

void CCDataManager::releaseData( const char* dataFile )
{
	if (m_mapDatas.find(dataFile) != m_mapDatas.end())
	{
		CCDataCollection* pDataCollection = m_mapDatas[dataFile];
		-- m_mapRefs[pDataCollection];
		if (m_mapRefs[pDataCollection] == 0)
		{
			pDataCollection->release();
			delete pDataCollection;
			m_mapDatas.erase(dataFile);
		}
	}
}

} // namespace

