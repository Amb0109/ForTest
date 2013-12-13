#ifndef _GAME_ENGINE_SCENE_H_
#define _GAME_ENGINE_SCENE_H_

#include "ge_include.h"

namespace ge
{

class GE_API GEObject;
class GE_API GEScene
{
protected:
	typedef std::map<int, GEObject*> GE_OBJECT_MAP;

public:
	GEScene();
	virtual ~GEScene();

public:
	virtual bool init();
	virtual void destory();
	virtual bool show();
	virtual bool hide();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

protected:
	GE_OBJECT_MAP object_map_;
};

} // namespace ge

#endif // _GAME_ENGINE_SCENE_H_