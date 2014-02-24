#include "geu_vertex.h"
#include "../common/ge_engine.h"

namespace ge
{

GEVertexDecl::GEVertexDecl()
: vertex_decl_map_()
{
}

GEVertexDecl::~GEVertexDecl()
{
	FOR_EACH (VERTEX_DECL_MAP, vertex_decl_map_, decl_itor)
	{
		_destory_vertex_decl(decl_itor->second);
		delete vertex_decl_map_[decl_itor->first];
		vertex_decl_map_[decl_itor->first] = NULL;
	}
	vertex_decl_map_.clear();
}

GEVertexDecl* GEVertexDecl::get_instance()
{
	static GEVertexDecl _global_ge_vertex_decl;
	return &_global_ge_vertex_decl;
}

bool GEVertexDecl::_init_vertex_decl( GE_VERTEX_DECL* out_decl, DWORD fvf )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	_destory_vertex_decl(out_decl);

	out_decl->fvf = fvf;

	int element_array_pos = 0;
	int vertex_buff_size = 0;
	_calc_vertex_element_array(fvf, element_array_pos, vertex_buff_size);
	out_decl->size = vertex_buff_size;

	HRESULT h_res = S_OK;
	h_res = p_d3d_device->CreateVertexDeclaration(vertex_element_, &(out_decl->decl));
	assert(SUCCEEDED(h_res));

	return SUCCEEDED(h_res);
}

void GEVertexDecl::_destory_vertex_decl( GE_VERTEX_DECL* out_decl )
{
	D3D_RELEASE((out_decl->decl));
	out_decl->fvf = NULL;
	out_decl->size = 0;
}

void GEVertexDecl::_calc_vertex_element_array( DWORD fvf, int& array_pos, int& mem_size )
{
	array_pos = -1;
	mem_size = 0;

	ZeroMemory(vertex_element_, sizeof(vertex_element_));
	_add_vertex_element(fvf, D3DFVF_XYZ, array_pos, mem_size);
	_add_vertex_element(fvf, D3DFVF_NORMAL, array_pos, mem_size);
	_add_vertex_element(fvf, D3DFVF_TEXCOUNT_MASK, array_pos, mem_size);
	_add_vertex_element(fvf, D3DFVF_DIFFUSE, array_pos, mem_size);
	_add_vertex_element(fvf, NULL, array_pos, mem_size);
}

bool GEVertexDecl::_add_vertex_element( DWORD fvf, DWORD add_fvf, int& array_pos, int& mem_offset )
{
	if (add_fvf != NULL && !(fvf & add_fvf)) return false;
	if (array_pos + 1 >= VERTEX_ELEMENT_MAX_CNT) return false;

	switch (add_fvf)
	{
	case D3DFVF_XYZ:
		{
			++ array_pos;
			vertex_element_[array_pos].Stream	= 0;
			vertex_element_[array_pos].Offset	= mem_offset;
			vertex_element_[array_pos].Method	= D3DDECLMETHOD_DEFAULT;
			vertex_element_[array_pos].Type		= D3DDECLTYPE_FLOAT3;
			vertex_element_[array_pos].Usage	= D3DDECLUSAGE_POSITION;
			vertex_element_[array_pos].UsageIndex = 0;
			mem_offset += sizeof(float) * 3;
		}
		break;
	case D3DFVF_DIFFUSE:
		{
			++ array_pos;
			vertex_element_[array_pos].Stream	= 0;
			vertex_element_[array_pos].Offset	= mem_offset;
			vertex_element_[array_pos].Method	= D3DDECLMETHOD_DEFAULT;
			vertex_element_[array_pos].Type		= D3DDECLTYPE_D3DCOLOR;
			vertex_element_[array_pos].Usage	= D3DDECLUSAGE_COLOR;
			vertex_element_[array_pos].UsageIndex = 0;
			mem_offset += sizeof(D3DCOLOR);
		}
		break;
	case D3DFVF_NORMAL:
		{
			++ array_pos;
			vertex_element_[array_pos].Stream	= 0;
			vertex_element_[array_pos].Offset	= mem_offset;
			vertex_element_[array_pos].Method	= D3DDECLMETHOD_DEFAULT;
			vertex_element_[array_pos].Type		= D3DDECLTYPE_FLOAT3;
			vertex_element_[array_pos].Usage	= D3DDECLUSAGE_NORMAL;
			vertex_element_[array_pos].UsageIndex = 0;
			mem_offset += sizeof(float) * 3;
		}
		break;
	case D3DFVF_TEXCOUNT_MASK:
		{
			++ array_pos;
			vertex_element_[array_pos].Stream	= 0;
			vertex_element_[array_pos].Offset	= mem_offset;
			vertex_element_[array_pos].Method	= D3DDECLMETHOD_DEFAULT;
			vertex_element_[array_pos].Type		= D3DDECLTYPE_FLOAT2;
			vertex_element_[array_pos].Usage	= D3DDECLUSAGE_TEXCOORD;
			vertex_element_[array_pos].UsageIndex = BYTE(((add_fvf & fvf) >> D3DFVF_TEXCOUNT_SHIFT) - 1);
			mem_offset += sizeof(float) * 2;
		}
		break;
		// TODO
	default:
		{
			++ array_pos;
			vertex_element_[array_pos].Stream	= 0xFF;
			vertex_element_[array_pos].Offset	= 0;
			vertex_element_[array_pos].Method	= 0;
			vertex_element_[array_pos].Type		= D3DDECLTYPE_UNUSED;
			vertex_element_[array_pos].Usage	= 0;
			vertex_element_[array_pos].UsageIndex = 0;
		}
	}

	return true;
}

bool GEVertexDecl::_create_vertex_decl( DWORD fvf )
{
	if (vertex_decl_map_.find(fvf) != vertex_decl_map_.end()) return true;
	vertex_decl_map_[fvf] = new GE_VERTEX_DECL;
	return _init_vertex_decl(vertex_decl_map_[fvf], fvf);
}

void GEVertexDecl::_release_vertex_decl( DWORD fvf )
{
	if (vertex_decl_map_.find(fvf) == vertex_decl_map_.end()) return;
	_destory_vertex_decl(vertex_decl_map_[fvf]);
	delete vertex_decl_map_[fvf];
}

GE_VERTEX_DECL* GEVertexDecl::_get_vertex_decl( DWORD fvf )
{
	if (vertex_decl_map_.find(fvf) == vertex_decl_map_.end()) return NULL;
	return vertex_decl_map_[fvf];
}

GE_VERTEX_DECL* GEVertexDecl::get_vertex_decl( DWORD fvf )
{
	GEVertexDecl* vertex_decl = GEVertexDecl::get_instance();
	if (!vertex_decl->_create_vertex_decl(fvf)) return false;
	return vertex_decl->_get_vertex_decl(fvf);
}



GE_VERTEX::GE_VERTEX()
: position_(0.f, 0.f, 0.f)
, normal_(0.f, 0.f, 0.f)
, texcoords_(0.f, 0.f)
, color_(0xffffffff)
, decl_(NULL)
{
}

GE_VERTEX::~GE_VERTEX()
{

}

bool GE_VERTEX::set_fvf( DWORD fvf )
{
	return set_decl(GEVertexDecl::get_vertex_decl(fvf));
}

bool GE_VERTEX::set_decl( GE_VERTEX_DECL* decl )
{
	if (decl == NULL) return false;
	if (!decl->is_valid()) return false;
	decl_ = decl;
	return true;
}

GE_VERTEX_DECL* GE_VERTEX::get_decl()
{
	return decl_;
}

void GE_VERTEX::set_position( float x, float y, float z )
{
	position_.x = x;
	position_.y = y;
	position_.z = z;
}

void GE_VERTEX::set_normal( float x, float y, float z )
{
	normal_.x = x;
	normal_.y = y;
	normal_.z = z;
}

void GE_VERTEX::set_texcoords( float u, float v )
{
	texcoords_.x = u;
	texcoords_.y = v;
}

void GE_VERTEX::set_color( D3DCOLOR color )
{
	color_ = color;
}

bool GE_VERTEX::pack( void* mem_buff, int size )
{
	if (mem_buff == NULL) return false;

	if (decl_ == NULL) return false;
	if (!decl_->is_valid()) return false;

	if(decl_->size != size) return false;

	int buff_offset = 0;
	memset(mem_buff, 0, size);
	if (decl_->fvf & D3DFVF_XYZ)
	{
		_append_data(mem_buff, buff_offset, (void*)&(position_), sizeof(D3DXVECTOR3));
	}

	if (decl_->fvf & D3DFVF_NORMAL)
	{
		_append_data(mem_buff, buff_offset, (void*)&(normal_), sizeof(D3DXVECTOR3));
	}

	if (decl_->fvf & D3DFVF_TEXCOUNT_MASK)
	{
		_append_data(mem_buff, buff_offset, (void*)&(texcoords_), sizeof(D3DXVECTOR2));
	}

	if (decl_->fvf & D3DFVF_DIFFUSE)
	{
		_append_data(mem_buff, buff_offset, (void*)&(color_), sizeof(D3DCOLOR));
	}

	assert(buff_offset == decl_->size);
	if (buff_offset != decl_->size) return false;
	return true;
}

bool GE_VERTEX::_append_data( void* mem_buff, int& mem_offset, void* p_data, int data_size )
{
	memcpy((char*)mem_buff + mem_offset, p_data, data_size);
	mem_offset += data_size;
	return true;
}



}


