#ifndef _CC_ATLAS_DATA_H_
#define _CC_ATLAS_DATA_H_

#include "../cocos/CCMacro.h"
#include "../support/CCSAXParser.h"
#include <stack>
#include <map>
#include <string>

namespace CC {

class CCAtlasMaker;
class CCAtlasPage;
struct CCAtlasData;

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

class  CC_DLL CCAtlasMaker : public CCSAXDelegator
{
public:
	static CCAtlasMaker* getInstance() { static CCAtlasMaker _globalCCAtlasMaker; return &_globalCCAtlasMaker;}

public:
	void startElement(void *ctx, const char* name, const char** atts);
	void endElement(void *ctx, const char* name);
	void textHandler(void *ctx, const char *ch, int len);

public:
	bool prase(CCAtlasPage* pAtlasPage, const char* plistData, int len);
	void clear();

private:
	CCAtlasPage*					m_pAtlasPage;
	CCAtlasDict						m_eCurrentDict;
	std::string						m_sCurrentDictKey;
	std::stack<CCSaxType>			m_stkTypeStack;
	std::stack<std::string>			m_stkKeyStack;
};

class CC_DLL CCAtlasPage
{
public:
	typedef std::map<std::string, CCAtlasData> ATLAS_FRAME_MAP;

public:
	bool loadFile(const char* filePath);
	bool load(const char* fileData, int len);
	
	void clear();

	CCAtlasData*			getAtlasData(const char* name);

public:
	ATLAS_FRAME_MAP			m_mapAtlasData;

	int						m_iWidth;
	int						m_iHeight;
	std::string				m_sBasePath;
	std::string				m_sPath;
	std::string				m_sImageFile;
};

struct CC_DLL CCAtlasData
{
	std::string		m_sName;
	int				m_iWidth;
	int				m_iHeight;
	int				m_iOriginalWidth;
	int				m_iOriginalHeight;
	int				m_iX;
	int				m_iY;
	float			m_fOffsetX;
	float			m_fOffsetY;

	CCAtlasPage*	m_pPage;
};

} // namespace

#endif // _CC_ATLAS_DATA_H_