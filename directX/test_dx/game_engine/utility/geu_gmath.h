#ifndef _GAME_ENGINE_UTILITY_GMATH_H_
#define _GAME_ENGINE_UTILITY_GMATH_H_

#include "../common/ge_include.h"

namespace ge
{

struct GE_API GE_IPOINT
{
	int x;
	int y;
};

struct GE_API GE_ISIZE
{
	int width;
	int height;
};

struct GE_API GE_IRECT : public RECT
{
	GE_IRECT();
	GE_IRECT(int left_, int top_, int right_, int bottom_);

	int width() {return right - left;}
	int height() {return bottom - top;}

	void move(int offset_x, int offset_y);
	void move_to(int pos_x, int pos_y);
};

struct GE_API GE_FPOINT
{
	float x;
	float y;
};

struct GE_API GE_FSIZE
{
	float width;
	float height;
};

struct GE_API GE_FRECT
{
	float left;
	float top;
	float right;
	float bottom;

	GE_FRECT();
	GE_FRECT(float left_, float top_, float right_, float bottom_);

	float width() {return right - left;}
	float height() {return bottom - top;}

	void move(float offset_x, float offset_y);
	void move_to(float pos_x, float pos_y);
};

}

#endif // _GAME_ENGINE_UTILITY_GMATH_H_