#include "CCTextureAtlas.h"
#include "../data/CCAtlasData.h"

namespace CC {

CCTextureAtlas::CCTextureAtlas()
: m_pAtlasPage(NULL)
, m_vecPaintVerties()
, m_pRenderObject(NULL)
, m_obTextureSize(CCSizeZero)
{
}

CCTextureAtlas::~CCTextureAtlas()
{
	release();
}

void CCTextureAtlas::addQuad( const CC_PAINT_QUAD& quad )
{
	m_vecPaintVerties.push_back(quad.tl);
	m_vecPaintVerties.push_back(quad.tr);
	m_vecPaintVerties.push_back(quad.br);
	m_vecPaintVerties.push_back(quad.bl);
}

void CCTextureAtlas::clearQuads()
{
	m_vecPaintVerties.clear();
}

} // namespace