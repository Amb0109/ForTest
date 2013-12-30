#ifndef _OJBECT_MODEL_TEST_H_
#define _OJBECT_MODEL_TEST_H_

#include "game_engine.h"

#define DEF_FVF_FORMAT (D3DFVF_XYZ | D3DFVF_TEX1)

class Panel2D : public ge::GEOPrimitive
{
public:
	Panel2D():ge::GEOPrimitive(){}
	virtual ~Panel2D(){}

public:
	virtual bool init();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

private:
	LPDIRECT3DTEXTURE9	p_texture_;
};

#endif // _OJBECT_MODEL_TEST_H_