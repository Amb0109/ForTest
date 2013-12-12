#include "ge_def.h"

namespace ge
{
GE_IRECT::GE_IRECT()
{
}

GE_IRECT::GE_IRECT( int left_, int top_, int right_, int bottom_ )
{
	top		= top_;
	bottom	= bottom_;
	left	= left_;
	right	= right_;
}

void GE_IRECT::move( int offset_x, int offset_y )
{
	top		+= offset_x;
	bottom	+= offset_x;
	left	+= offset_y;
	right	+= offset_y;
}

void GE_IRECT::move_to( int pos_x, int pos_y )
{
	right	+= pos_x - left;
	left	= pos_x;
	bottom	+= pos_y - top;
	top		= pos_y;
}



}


