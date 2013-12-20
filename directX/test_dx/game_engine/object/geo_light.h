#ifndef _GAME_ENGINE_OBJECT_LIGHT_H_
#define _GAME_ENGINE_OBJECT_LIGHT_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"

namespace ge
{

class GE_API GEOLight : public GEObject
{
public:
	GEOLight();
	virtual ~GEOLight();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

protected:
	D3DLIGHT9	light_;
	D3DMATERIAL9 material_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_LIGHT_H_