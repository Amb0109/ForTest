#include "geo_mesh.h"
#include "../common/ge_engine.h"
#include "../render/ge_render.h"

namespace ge
{
	
GEOMesh::GEOMesh()
{
}

GEOMesh::~GEOMesh()
{

}

bool GEOMesh::init()
{
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	D3DXCreateTeapot(p_device, &p_mesh_, 0);
	init_effect();

	return true;
}


bool GEOMesh::init_effect()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;

	effect_.init("./shader/diffuse.fx");
	LPD3DXEFFECT p_fx = effect_.get_fx();
	if(p_fx == NULL) return false;

	D3DXHANDLE h_tech_main = p_fx->GetTechniqueByName("TechMain");
	p_fx->SetTechnique(h_tech_main);

	D3DXHANDLE h_ambient_mtrl = p_fx->GetParameterByName(0, "AmbientMtrl");
	D3DXHANDLE h_diffuse_mtrl = p_fx->GetParameterByName(0, "DiffuseMtrl");
	D3DXHANDLE h_light_direction = p_fx->GetParameterByName(0, "LightDirection");

	D3DXVECTOR4 directionToLight(-0.57f, 0.57f, -0.57f, 0.0f);
	p_fx->SetVector(h_light_direction, &directionToLight);

	D3DXVECTOR4 ambientMtrl(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXVECTOR4 diffuseMtrl(0.0f, 0.0f, 1.0f, 1.0f);
	p_fx->SetVector(h_ambient_mtrl, &ambientMtrl);
	p_fx->SetVector(h_diffuse_mtrl, &diffuseMtrl);
	return true;
}

bool GEOMesh::update_effect()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_device();
	if(p_d3d_device == NULL) return false;
	GERender* p_render = GEEngine::get_instance()->get_render();
	if(p_render == NULL) return false;
	LPD3DXEFFECT p_fx = effect_.get_fx();
	if(p_fx == NULL) return false;

	D3DXHANDLE h_wvm = p_fx->GetParameterByName(0, "gWV");
	D3DXHANDLE h_wvpm = p_fx->GetParameterByName(0, "gWVP");

	D3DXMATRIX view_matrix = world_matrix_;
	view_matrix = view_matrix * p_render->get_view_matrix();
	D3DXMATRIX view_proj_matrix = world_matrix_;
	view_proj_matrix = view_proj_matrix * p_render->get_view_matrix() * p_render->get_proj_matrix();

	p_fx->SetMatrix(h_wvm, &view_matrix);
	p_fx->SetMatrix(h_wvpm, &view_proj_matrix);
	p_fx->CommitChanges();

	return true;
}

void GEOMesh::destory()
{
	SAFE_RELEASE(p_mesh_);
}

void GEOMesh::update( time_t time_elapsed )
{
	transform_.px = -3.f;
	transform_.rx = D3DX_PI / 2.34f;
	transform_.ry += time_elapsed / 1000.f;
	transform_.sx = 1.f;
	transform_.sy = 1.f;
}

void GEOMesh::render( time_t time_elapsed )
{
	GEObject::render(time_elapsed);

	LPDIRECT3DDEVICE9 p_d3d_device = ge::GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return;
	LPD3DXEFFECT p_fx = effect_.get_fx();
	if(p_fx == NULL) return;

	update_effect();
	
	UINT pass_num = 0;
	p_fx->Begin(&pass_num, 0);
	for(UINT i = 0; i < pass_num; ++i)
	{
		p_fx->BeginPass(i);

		p_mesh_->DrawSubset(0);

		p_fx->EndPass();
	}
	p_fx->End();
}

}