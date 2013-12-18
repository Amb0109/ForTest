#include "model_test.h"


ModelTest::ModelTest()
:model_rotation_y(0.f)
{

}

void ModelTest::update( time_t time_elapsed )
{

}

void ModelTest::render( time_t time_elapsed )
{
	LPDIRECT3DDEVICE9 p_d3d_device = ge::GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return;

	D3DXMATRIX rotation_matrix_x;
	D3DXMATRIX rotation_matrix_y;
	D3DXMatrixRotationX(&rotation_matrix_x, D3DX_PI / 2.34f);
	D3DXMatrixRotationY(&rotation_matrix_y, model_rotation_y);
	model_rotation_y += time_elapsed / 1000.f;

	D3DXMATRIX rotation_matrix = rotation_matrix_x * rotation_matrix_y;
	p_d3d_device->SetTransform(D3DTS_WORLD, &rotation_matrix);

	p_d3d_device->SetVertexShader(NULL);
	ge::GEOModel::render(time_elapsed);
}