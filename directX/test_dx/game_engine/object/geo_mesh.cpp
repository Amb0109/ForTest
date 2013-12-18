#include "geo_mesh.h"
#include "../common/ge_engine.h"
#include "../render/ge_render.h"

namespace ge
{
	
GEOMesh::GEOMesh()
{
	model_rotation_y = 0;
}

GEOMesh::~GEOMesh()
{

}

bool GEOMesh::init()
{
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	D3DXCreateTeapot(p_device, &p_mesh_, 0);

	test_shader.create("./shader/transform.hlsl");
	const char* err_msg = test_shader.get_compile_error();
	
	return true;
}

void GEOMesh::destory()
{
	SAFE_RELEASE(p_mesh_);
}

void GEOMesh::update( time_t time_elapsed )
{

}

void GEOMesh::render( time_t time_elapsed )
{
	LPDIRECT3DDEVICE9 p_d3d_device = ge::GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return;

	GERender* p_render = GEEngine::get_instance()->get_render();
	if(p_render == NULL) return ;

	D3DXMATRIX rotation_matrix_x;
	D3DXMATRIX rotation_matrix_y;
	D3DXMatrixRotationX(&rotation_matrix_x, D3DX_PI / 2.34f);
	D3DXMatrixRotationY(&rotation_matrix_y, model_rotation_y);
	model_rotation_y += time_elapsed / 1000.f;

	D3DXMATRIX trans_matrix;
	D3DXMatrixTranslation(&trans_matrix, -3.f, 0.0f, 0.0f);
	D3DXMATRIX word_matrix = rotation_matrix_x * rotation_matrix_y * trans_matrix;
	p_d3d_device->SetTransform(D3DTS_WORLD, &word_matrix);

	D3DXMATRIX view_proj_matrix;
	//GERender* p_render = GEEngine::get_render();
	view_proj_matrix = p_render->get_view_matrix() * p_render->get_proj_matrix();
	//h_res = p_constent_table_->SetMatrix(p_d3d_device, h_wvm, &p_render->get_word_view_matrix());

	test_shader.test_func(word_matrix * view_proj_matrix);
	
	p_mesh_->DrawSubset(0);
}

}