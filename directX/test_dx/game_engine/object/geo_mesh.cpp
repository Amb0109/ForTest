#include "geo_mesh.h"
#include "../common/ge_engine.h"
#include "../render/ge_render.h"
#include "../render/ger_effect.h"
#include "../render/ger_material.h"
#include "../utility/geu_vertex.h"

namespace ge
{
	
GEOMesh::GEOMesh()
:p_mesh_(NULL),
p_effect_(NULL),
p_material_(NULL)
{
}

GEOMesh::~GEOMesh()
{
	destory();
}

bool GEOMesh::create_mesh( GE_VERTEX_DECL* vertex_decl, int vertex_cnt, int face_cnt )
{
	LPDIRECT3DDEVICE9 p_d3d_device = ge::GEEngine::get_device();
	if (p_d3d_device == NULL) return false;

	if (vertex_decl == NULL) return false;
	if (!vertex_decl->is_valid()) return false;

	D3DVERTEXELEMENT9 vertex_element[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(vertex_decl->fvf, vertex_element);
	vertex_size_ = vertex_decl->size;
	if (vertex_size_ <= 0) return false;

	vertex_cnt_ = vertex_cnt;
	face_cnt_ = face_cnt;

	this->destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateMesh(
		face_cnt, vertex_cnt,
		D3DXMESH_MANAGED,
		vertex_element,
		p_d3d_device,
		&p_mesh_);

	if (FAILED(h_res))
	{
		destory();
	}

	return SUCCEEDED(h_res);
}

bool GEOMesh::set_vertices( GE_VERTEX* vertex_array, int vertex_cnt )
{
	if (p_mesh_ == NULL) return false;
	if (vertex_array == NULL) return false;
	if (vertex_cnt != vertex_cnt_) return false;

	bool b_ret = true;
	char* vertex_buff = NULL;
	p_mesh_->LockVertexBuffer(0, (void**)&vertex_buff);
	for (int i=0; i < vertex_cnt; ++i)
	{
		bool b_ret = true;
		b_ret = vertex_array[i].pack(vertex_buff, vertex_size_);
		if (!b_ret)
		{
			b_ret = false;
			break;
		}

		vertex_buff += vertex_size_;
	}
	p_mesh_->UnlockVertexBuffer();
	return b_ret;
}

bool GEOMesh::set_indices( WORD* index_array, int index_cnt )
{
	if (p_mesh_ == NULL) return false;
	if (index_array == NULL) return false;
	if (index_cnt != face_cnt_ * 3) return false;

	WORD* index_buff = NULL;
	p_mesh_->LockIndexBuffer(0, (void**)&index_buff);
	for (int i=0; i < index_cnt; ++i)
	{
		index_buff[i] = index_array[i];
	}
	p_mesh_->UnlockIndexBuffer();
	return true;
}

bool GEOMesh::create_mesh_from_file( const char* file_name )
{
	if (file_name == NULL) return false;
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	this->destory();

	HRESULT h_res = S_OK;
	h_res = D3DXLoadMeshFromX(
		file_name,
		D3DXMESH_MANAGED,
		p_device,
		NULL,	// Adjacency
		NULL,	// Materials
		NULL,	// Effect Instances
		0,		// Materials Number
		&p_mesh_);
	
	if (SUCCEEDED(h_res))
		return _get_infos_from_mesh();
	else
		return false;
}

bool GEOMesh::save_mesh_to_file( const char* file_name )
{
	return true;
}

bool GEOMesh::set_effect( GEREffect* p_effect )
{
	if (p_effect == NULL) return false;
	p_effect_ = p_effect;
	return true;
}

bool GEOMesh::set_material( GE_MATERIAL* p_material )
{
	if (p_material == NULL) return false;
	p_material_ = p_material;
	return true;
}

bool GEOMesh::init()
{
	if (p_effect_ != NULL)
		p_effect_->init();
	
	return true;
}

void GEOMesh::destory()
{
	D3D_RELEASE(p_mesh_);
	p_effect_ = NULL;
	
	vertex_cnt_		= 0;
	face_cnt_		= 0;
	vertex_size_	= 0;
}

void GEOMesh::update( time_t delta )
{
}

void GEOMesh::render( time_t delta )
{
	GEObject::render(delta);

	LPDIRECT3DDEVICE9 p_d3d_device = ge::GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return;

	if (p_material_ == NULL)
	{
			GE_MATERIAL::use_default_material();
	}
	else
	{
		p_d3d_device->SetMaterial(p_material_->get_d3d_material());
	}

	if (p_effect_ == NULL)
	{
		this->on_render(delta);
	}
	else
	{
		p_effect_->render(this, delta);
	}
}

void GEOMesh::on_render( time_t delta )
{
	if(p_mesh_ == NULL) return;
	p_mesh_->DrawSubset(0);
}

bool GEOMesh::create_mesh_box( float width, float height, float depth )
{
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	this->destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateBox(p_device, width, height, depth, &p_mesh_, 0);
	
	if (SUCCEEDED(h_res))
		return _get_infos_from_mesh();
	else
		return false;
}

bool GEOMesh::create_mesh_cylinder( float radius1, float radius2, float length, int slices, int stacks )
{
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	this->destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateCylinder(p_device, radius1, radius2, length, slices, stacks, &p_mesh_, 0);

	if (SUCCEEDED(h_res))
		return _get_infos_from_mesh();
	else
		return false;
}

bool GEOMesh::create_mesh_sphere( float radius, int slices, int stacks )
{
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	this->destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateSphere(p_device, radius, slices, stacks, &p_mesh_, 0);

	if (SUCCEEDED(h_res))
		return _get_infos_from_mesh();
	else
		return false;
}

bool GEOMesh::create_mesh_teapot()
{
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	this->destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateTeapot(p_device, &p_mesh_, 0);

	if (SUCCEEDED(h_res))
		return _get_infos_from_mesh();
	else
		return false;
}

bool GEOMesh::create_mesh_torus( float in_radius, float out_radius, int sides, int rings )
{
	LPDIRECT3DDEVICE9 p_device = ge::GEEngine::get_device();
	if (p_device == NULL) return false;

	this->destory();

	HRESULT h_res = S_OK;
	h_res = D3DXCreateTorus(p_device, in_radius, out_radius, sides, rings, &p_mesh_, 0);

	if (SUCCEEDED(h_res))
		return _get_infos_from_mesh();
	else
		return false;
}

bool GEOMesh::_get_infos_from_mesh()
{
	if (p_mesh_ != NULL)
	{
		vertex_cnt_ = (int)p_mesh_->GetNumVertices();
		face_cnt_ = (int)p_mesh_->GetNumFaces();
		vertex_size_ = (int)p_mesh_->GetNumBytesPerVertex();
		return true;
	}
	return false;
}

}