#ifndef _CC_TEXTURE_ATLAS_H_
#define _CC_TEXTURE_ATLAS_H_

#include "../cocos/CCMacro.h"
#include "../cocos/CCGeometry.h"
#include <vector>

namespace CC {

class CCAtlasPage;

struct CC_DLL CC_PAINT_VERTEX
{
	float x, y, z;
	unsigned color;
	float u, v;

	CC_PAINT_VERTEX (){}
	CC_PAINT_VERTEX (float _x, float _y, float _z, unsigned _color, float _u, float _v)
		: x(_x), y(_y), z(_z)
		, color(_color)
		, u(_u), v(_v){}
};

struct CC_DLL CC_PAINT_QUAD
{
	CC_PAINT_VERTEX tl;
	CC_PAINT_VERTEX tr;
	CC_PAINT_VERTEX bl;
	CC_PAINT_VERTEX br;
};

class CC_DLL CCTextureAtlas
{
public:
	CCTextureAtlas();
	virtual ~CCTextureAtlas();

public:
	bool init(CCAtlasPage* atlasPage);
	void release();

	// vertex
	void addQuad(const CC_PAINT_QUAD& quad);
	void clearQuads();
	void drawQuads();

	// texture
	bool initTexture(const char* texturePath);
	void releaseTexture();

private:
	MEMBER_GET_PTR(CCAtlasPage*, m_pAtlasPage, AtlasPage);

	// vertex
	std::vector<CC_PAINT_VERTEX>		m_vecPaintVerties;

	// texture
	MEMBER_GET_PTR(void*, m_pRenderObject, RenderObject);
	MEMBER_GET_CREF(CCSize, m_obTextureSize, TextureSize);
};

} // namespace

#endif // _CC_TEXTURE_ATLAS_H_