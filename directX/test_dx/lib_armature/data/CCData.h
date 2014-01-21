#ifndef _CC_DATA_LOADER_H_
#define _CC_DATA_LOADER_H_

#include "../cocos/CCMacro.h"
#include <string>
#include <vector>

#include "json/json.h"

namespace CC {

class CCArmatureData;
class CCAnimationData;
class CCTextureData;
class CCAtlasPage;
struct CCAtlasData;
class CCDataCollection;

typedef std::map<std::string, int> DATA_NAME_MAP;

class  CC_DLL CCBaseData
{
public:
	CCBaseData();

public:
	bool		load(Json::Value& json, CCDataCollection* data);
	void		copy(const CCBaseData* node_data);
	void		subtract(CCBaseData *from, CCBaseData *to, bool limit);

public:
	float		x;
	float		y;
	int			zOrder;
	float		scaleX;
	float		scaleY;
	float		skewX;
	float		skewY;

	float		tweenRotate;

	bool		isUseColorInfo;
	int			a, r, g, b;
};


class CC_DLL CCDataCollection
{
public:
	CCDataCollection();
	virtual ~CCDataCollection();

public:
	bool				loadFile(const char* filePath);
	void				release();

	void				setPositionScale(float scale) { m_sPositionScale = scale; }

	std::string&		getName() { return m_sName; }
	
	CCArmatureData*		getArmatureData(int index = 0);
	CCArmatureData*		getArmatureDataByName(const char* name);
	
	CCAnimationData*	getAnimationData(int index = 0);
	CCAnimationData*	getAnimationDataByName(const char* name);
	
	CCTextureData*		getTextureData(int index = 0);
	CCTextureData*		getTextureDataByName(const char* name);

	CCAtlasPage*		getAtlasPage(int index = 0);
	
	CCAtlasData*		getAtlasData(const char* atlasName);

protected:
	bool				_load(const char* fileData);

public:
	std::vector<CCArmatureData*>	m_vecArmatureDataList;
	std::vector<CCAnimationData*>	m_vecAnimationDataList;
	std::vector<CCTextureData*>		m_vecTextureDataList;
	std::vector<CCAtlasPage*>		m_vecAtlasPagesList;
	DATA_NAME_MAP					m_mapArmatureDataMap;
	DATA_NAME_MAP					m_mapAnimationDataMap;
	DATA_NAME_MAP					m_mapTextureDataMap;

	std::string						m_sName;
	float							m_fVersion;
	float							m_fContentScale;
	float							m_sPositionScale;
	std::string						m_sBasePath;
	std::string						m_sPath;
};

class CC_DLL CCDataManager
{
public:
	typedef std::map<std::string, CCDataCollection*> DATA_MAP;
	typedef std::map<CCDataCollection*, int> REFERENCE_MAP;
public:
	CCDataManager();
	virtual ~CCDataManager();

	static CCDataManager* getInstance();

public:
	CCDataCollection*	getData(const char* dataFile);
	void				releaseData(const char* dataFile);

private:
	DATA_MAP			m_mapDatas;
	REFERENCE_MAP		m_mapRefs;
};

} // namespace

#endif // _CC_DATA_LOADER_H_