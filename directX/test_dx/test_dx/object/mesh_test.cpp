#include "mesh_test.h"


void MeshTest::test_mesh_factory( int mesh_seed )
{
	mesh_seed_ = mesh_seed;
	switch(mesh_seed)
	{
	case 0:
		{
			create_mesh_teapot();

			EffectTest* p_effect = new EffectTest();
			p_effect->init();
			set_effect(p_effect);
		}
		break;
	case 1:
		{
			create_mesh_teapot();
		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}

	transform_.px = float(rand() % 8 - 4);
	transform_.py = float(rand() % 8 - 4);
	transform_.pz = float(rand() % 3 - 1);

	transform_.sx = (rand() % 5) / 10.f + 0.5f;
	transform_.sy = (rand() % 5) / 10.f + 0.5f;
	transform_.sz = (rand() % 5) / 10.f + 0.5f;

	r_x_ = (rand() % 100) / 100.f;
	r_y_ = (rand() % 100) / 100.f;
	r_z_ = (rand() % 100) / 100.f;
}

void MeshTest::update( time_t time_elapsed )
{
	transform_.rx += (time_elapsed / 1000.f) * r_x_;
	transform_.ry += (time_elapsed / 1000.f) * r_y_;
	transform_.rz += (time_elapsed / 1000.f) * r_z_;
}
