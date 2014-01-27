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
	DLL_MANAGE_CLASS(GEOArmature);

	GEOArmature();
	virtual ~GEOArmature();

public:
	virtual bool init();
	virtual void release();

	virtual void update(time_t delta);
	virtual void render(time_t delta);

private:
	CC::CCArmature* ptr_armature_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_ARMATURE_H_