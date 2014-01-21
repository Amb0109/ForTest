#ifndef _GAME_ENGINE_OBJECT_ARMATURE_H_
#define _GAME_ENGINE_OBJECT_ARMATURE_H_

#include "../common/ge_include.h"
#include "ge_object.h"
#include "ge_object_def.h"
#include "geo_primitive.h"

#include "CCInclude.h"


namespace ge
{

class GE_API GEOArmature : public GEObject
{
public:
	GEOArmature();
	virtual ~GEOArmature();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

private:
	CC::CCArmature* ptr_armature_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_ARMATURE_H_