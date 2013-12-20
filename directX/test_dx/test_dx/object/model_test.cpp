#include "model_test.h"


ModelTest::ModelTest()
{

}

void ModelTest::update( time_t time_elapsed )
{
	transform_.rx = D3DX_PI / 2.34f;
	transform_.ry += time_elapsed / 1000.f;
}
