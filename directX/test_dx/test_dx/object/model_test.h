#ifndef _OJBECT_MODEL_TEST_H_
#define _OJBECT_MODEL_TEST_H_

#include "game_engine.h"

#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEXCOORDSIZE1(0))
//#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class ModelTest : public ge::GEOModel
{
public:
	ModelTest();

public:
	virtual void update(time_t time_elapsed);

protected:
};

#endif // _OJBECT_MODEL_TEST_H_