#include "CCAtlasData.h"
#include "../cocos/CCUtils.h"
#include "../support/tinyxml2/tinyxml2.h"
#include <assert.h>
#include <algorithm>

namespace CC {

void CCAtlasMaker::startElement( void *ctx, const char* name, const char** atts )
{
	if (strcmp(name, "dict") == 0)
	{
		m_stkTypeStack.push(TYPE_DICT);
		switch (m_stkKeyStack.size())
		{
		case 0:
			{
				m_eCurrentDict = DICT_MAIN;
			}
			break;
		case 1:
			{
				const char* dict_key = m_stkKeyStack.top().c_str();
				if (strcmp(dict_key, "frames") == 0)
				{
					m_eCurrentDict = DICT_FRAMES;
				}
				else if (strcmp(dict_key, "metadata") == 0)
				{
					m_eCurrentDict = DICT_METADATA;
				}
				else if (strcmp(dict_key, "texture") == 0)
				{
					m_eCurrentDict = DICT_TEXTURE;
				}
				else
				{
					m_eCurrentDict = DICT_NONE;
				}
			}
			break;
		case 2:
			if (m_eCurrentDict == DICT_FRAMES)
			{
				m_eCurrentDict = DICT_FRAME;
			}
			break;
		default:
			m_eCurrentDict = DICT_NONE;
		}

		if (!m_stkKeyStack.empty()) m_sCurrentDictKey = m_stkKeyStack.top();
		else m_sCurrentDictKey = "";

		m_stkKeyStack.push("");
	}
	else if (strcmp(name, "key") == 0)
	{
		m_stkTypeStack.push(TYPE_KEY);
	}
	else if (strcmp(name, "integer") == 0)
	{
		m_stkTypeStack.push(TYPE_INT);
	}
	else if (strcmp(name, "real") == 0)
	{
		m_stkTypeStack.push(TYPE_REAL);
	}
	else if (strcmp(name, "string") == 0)
	{
		m_stkTypeStack.push(TYPE_STRING);
	}
	else if (strcmp(name, "array") == 0)
	{
		m_stkTypeStack.push(TYPE_ARRAY);
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
		if(!m_stkTypeStack.empty() && TYPE_DICT == m_stkTypeStack.top())
		{
			m_stkTypeStack.pop();
			m_stkKeyStack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "key") == 0)
	{
		if(!m_stkTypeStack.empty() && TYPE_KEY == m_stkTypeStack.top())
		{
			m_stkTypeStack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "integer") == 0)
	{
		if(!m_stkTypeStack.empty() && TYPE_INT == m_stkTypeStack.top())
		{
			m_stkTypeStack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "real") == 0)
	{
		if(!m_stkTypeStack.empty() && TYPE_REAL == m_stkTypeStack.top())
		{
			m_stkTypeStack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "string") == 0)
	{
		if(!m_stkTypeStack.empty() && TYPE_STRING == m_stkTypeStack.top())
		{
			m_stkTypeStack.pop();
		} else {
			assert(false);
			return;
		}
	}
	else if (strcmp(name, "array") == 0)
	{
		if(!m_stkTypeStack.empty() && TYPE_ARRAY == m_stkTypeStack.top())
		{
			m_stkTypeStack.pop();
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
	if (m_stkTypeStack.empty()) return;
	if (m_pAtlasPage == NULL) return;

	if (TYPE_KEY == m_stkTypeStack.top())
	{
		m_stkKeyStack.pop();
		m_stkKeyStack.push(ch);
		return;
	}

	const char* curKey = m_stkKeyStack.top().c_str();
	switch (m_eCurrentDict)
	{
	case DICT_FRAME:
		{
			if (m_pAtlasPage->m_mapAtlasData.find(m_sCurrentDictKey) == m_pAtlasPage->m_mapAtlasData.end())
			{
				CCAtlasData& atlasData = m_pAtlasPage->m_mapAtlasData[m_sCurrentDictKey];
				atlasData.m_sName = m_sCurrentDictKey;
				atlasData.m_pPage = m_pAtlasPage;
			}

			CCAtlasData& atlasData = m_pAtlasPage->m_mapAtlasData[m_sCurrentDictKey];
			if (strcmp(curKey, "width") == 0) tinyxml2::XMLUtil::ToInt(ch, &(atlasData.m_iWidth));
			else if (strcmp(curKey, "height") == 0) tinyxml2::XMLUtil::ToInt(ch, &(atlasData.m_iHeight));
			else if (strcmp(curKey, "x") == 0) tinyxml2::XMLUtil::ToInt(ch, &(atlasData.m_iX));
			else if (strcmp(curKey, "y") == 0) tinyxml2::XMLUtil::ToInt(ch, &(atlasData.m_iY));
			else if (strcmp(curKey, "originalWidth") == 0) tinyxml2::XMLUtil::ToInt(ch, &(atlasData.m_iOriginalWidth));
			else if (strcmp(curKey, "originalHeight") == 0) tinyxml2::XMLUtil::ToInt(ch, &(atlasData.m_iOriginalHeight));
			else if (strcmp(curKey, "offsetX") == 0) tinyxml2::XMLUtil::ToFloat(ch, &(atlasData.m_fOffsetX));
			else if (strcmp(curKey, "offsetY") == 0) tinyxml2::XMLUtil::ToFloat(ch, &(atlasData.m_fOffsetY));
		}
		break;
	case DICT_METADATA:
		{
			if (strcmp(curKey, "textureFileName") == 0) m_pAtlasPage->m_sImageFile = m_pAtlasPage->m_sBasePath + ch;
		}
		break;
	case DICT_TEXTURE:
		{
			if (strcmp(curKey, "width") == 0) tinyxml2::XMLUtil::ToInt(ch, &(m_pAtlasPage->m_iWidth));
			else if (strcmp(curKey, "height") == 0) tinyxml2::XMLUtil::ToInt(ch, &(m_pAtlasPage->m_iHeight));
		}
		break;
	default:
		break;
	}
}

bool CCAtlasMaker::prase(CCAtlasPage* pAtlasPage, const char* plistData, int len )
{
	CCSAXParser parser;
	parser.setDelegator(this);
	if(!parser.init("UTF-8")) return false;
	m_pAtlasPage = pAtlasPage;
	return parser.parse(plistData, len);
}

void CCAtlasMaker::clear()
{
	m_pAtlasPage = NULL;
	m_eCurrentDict = DICT_NONE;
	m_sCurrentDictKey.clear();
	while(!m_stkTypeStack.empty()) m_stkTypeStack.pop();
	while(!m_stkKeyStack.empty()) m_stkKeyStack.pop();
}

bool CCAtlasPage::loadFile( const char* filePath )
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
	bool ret = load(pFileContent, size);
	delete pFileContent;
	return ret;
}

bool CCAtlasPage::load(const char* fileData, int len)
{
	CCAtlasMaker* pAtlasMaker = CCAtlasMaker::getInstance();
	pAtlasMaker->clear();
	return pAtlasMaker->prase(this, fileData, len);
}

void CCAtlasPage::clear()
{
	m_mapAtlasData.clear();
	m_iWidth = 0;
	m_iHeight = 0;
	m_sBasePath.clear();
}

CCAtlasData* CCAtlasPage::getAtlasData( const char* name )
{
	if (m_mapAtlasData.find(name) == m_mapAtlasData.end()) return NULL;
	return &(m_mapAtlasData[name]);
}

} // namespace