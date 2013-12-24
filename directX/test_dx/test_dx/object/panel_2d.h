#ifndef _OJBECT_MODEL_TEST_H_
#define _OJBECT_MODEL_TEST_H_

#include "game_engine.h"

#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2)

class Panel2D : public ge::GEOModel
{
public:
	Panel2D():ge::GEOModel(){}
	virtual ~Panel2D(){}

public:
	virtual bool init();

	virtual void update(time_t time_elapsed);
};

#endif // _OJBECT_MODEL_TEST_H_