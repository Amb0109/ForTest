#ifndef _RENDER_EFFECT_TEST_H_
#define _RENDER_EFFECT_TEST_H_

#include "game_engine.h"

class EffectTest : public ge::GEREffect
{

public:
	virtual bool init();
	virtual void render(ge::GEObject* obj, time_t time_elapsed);
};

#endif //_SCENE_SCENE_TEST_H_