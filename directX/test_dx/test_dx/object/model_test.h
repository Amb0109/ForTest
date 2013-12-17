#ifndef _OJBECT_MODEL_TEST_H_
#define _OJBECT_MODEL_TEST_H_

#include "game_engine.h"

class ModelTest : public ge::GEOModel
{
public:
	ModelTest();

public:
	virtual void update(time_t time_elapsed);

protected:
	float			model_rotation_y;
};

#endif // _OJBECT_MODEL_TEST_H_