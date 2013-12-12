#ifndef _GAME_ENGINE_DEFINE_H_
#define _GAME_ENGINE_DEFINE_H_

#include "ge_include.h"

namespace ge
{

struct GE_API GE_IRECT : public RECT
{
	GE_IRECT();
	GE_IRECT(int left_, int top_, int right_, int bottom_);

	int width() {return right - left;}
	int height() {return bottom - top;}

	void move(int offset_x, int offset_y);
	void move_to(int pos_x, int pos_y);
};

}

#endif // _GAME_ENGINE_DEFINE_H_