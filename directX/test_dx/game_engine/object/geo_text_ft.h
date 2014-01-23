#ifndef _GAME_ENGINE_OBJECT_TEXT_FT_H_
#define _GAME_ENGINE_OBJECT_TEXT_FT_H_

#include "ge_object.h"
#include "../utility/geu_gmath.h"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace ge
{

class GE_API GEFreeType
{
protected:
	GEFreeType();
	virtual ~GEFreeType();

	static GEFreeType* get_instance();

public:
	bool init();
	void destory();

	bool create_font();

private:
	FT_Library freetype_;
};


class GE_API GEOTextFT : public GEObject
{
	DLL_MANAGE_CLASS(GEOTextFT);

public:
	//GEOTextFT();
	//virtual ~GEOTextFT();

public:
	//virtual void render(time_t delta);

public:
	std::string		text_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_FT_H_