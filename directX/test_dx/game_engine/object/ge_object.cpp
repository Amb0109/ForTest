#include "ge_object.h"

namespace ge
{

GEObject::GEObject()
{
	memset(&transform_, 0, sizeof(transform_));
	transform_.sx = 1.f;
	transform_.sy = 1.f;
	transform_.sz = 1.f;

	_calc_world_matrix();
}

GEObject::~GEObject()
{

}

bool GEObject::init()
{
	return true;
}

void GEObject::destory()
{

}

void GEObject::update( time_t time_elapsed )
{

}

void GEObject::render( time_t time_elapsed )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return;
	
	_calc_world_matrix();

	p_d3d_device->SetTransform(D3DTS_WORLD, &world_matrix_);
}

void GEObject::_calc_world_matrix()
{
	// 位置
	D3DXMatrixTranslation(&trans_matrix_, transform_.px, transform_.py, transform_.pz);

	// 旋转
	D3DXMatrixRotationX(&rotatex_matrix_, transform_.rx);
	D3DXMatrixRotationY(&rotatey_matrix_, transform_.ry);
	D3DXMatrixRotationZ(&rotatez_matrix_, transform_.rz);

	// 缩放
	D3DXMatrixScaling(&scale_matrix_, transform_.sx, transform_.sy, transform_.sz);

	// TODO 顺序问题
	world_matrix_ = scale_matrix_;
	world_matrix_ = world_matrix_ * rotatex_matrix_;
	world_matrix_ = world_matrix_ * rotatey_matrix_;
	world_matrix_ = world_matrix_ * rotatez_matrix_;
	world_matrix_ = world_matrix_ * trans_matrix_;
}

void GEObject::on_update( time_t time_elapsed )
{

}

void GEObject::on_render( time_t time_elapsed )
{

}

}