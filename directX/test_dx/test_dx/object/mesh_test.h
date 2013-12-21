#ifndef _OJBECT_MESH_TEST_H_
#define _OJBECT_MESH_TEST_H_

#include "game_engine.h"
#include "../render/effect_test.h"

class MeshTest : public ge::GEOMesh
{
public:
	void test_mesh_factory(int mesh_seed);

public:
	virtual void update(time_t time_elapsed);

private:
	int mesh_seed_;

	float r_x_;
	float r_y_;
	float r_z_;
};

#endif // _OJBECT_MESH_TEST_H_