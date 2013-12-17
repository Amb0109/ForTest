#ifndef _GAME_ENGINE_UTILITY_GMATH_H_
#define _GAME_ENGINE_UTILITY_GMATH_H_

#include "../common/ge_include.h"


namespace ge
{

struct GE_VERTEX
{
	float x, y, z;

	GE_VERTEX():x(0), y(0), z(0) {}
	GE_VERTEX(float x_, float y_, float z_):x(x_), y(y_), z(z_) {}
	static DWORD FVF() {return D3DFVF_XYZ;}
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

struct GE_API GE_FRECT
{
	float left;
	float top;
	float right;
	float bottom;
};

}

#endif // _GAME_ENGINE_UTILITY_GMATH_H_