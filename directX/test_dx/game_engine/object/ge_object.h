#ifndef _GAME_ENGINE_OBJECT_H_
#define _GAME_ENGINE_OBJECT_H_

#include "../common/ge_include.h"
#include "ge_object_def.h"

namespace ge
{

class GE_API GEObject
{
public:
	GEObject();
	virtual ~GEObject();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

protected:
	GEObjectType type_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_H_