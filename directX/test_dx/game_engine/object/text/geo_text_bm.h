#ifndef _GAME_ENGINE_OBJECT_TEXT_BM_H_
#define _GAME_ENGINE_OBJECT_TEXT_BM_H_

#include "../ge_object.h"
#include "geo_text.h"

namespace ge
{

class GE_API GEOTextBM : public GEOText
{
	DLL_MANAGE_CLASS(GEOTextBM);

public:
	GEOTextBM();
	virtual ~GEOTextBM();

};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_TEXT_BM_H_