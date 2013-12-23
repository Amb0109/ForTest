#ifndef _OJBECT_MODEL_TEST_H_
#define _OJBECT_MODEL_TEST_H_

#include "game_engine.h"

#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)
//#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class ModelTest : public ge::GEOModel
{

public:
	virtual void update(time_t time_elapsed);

};

#endif // _OJBECT_MODEL_TEST_H_