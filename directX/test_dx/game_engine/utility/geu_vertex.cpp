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
	_update_vertex_element(fvf, D3DFVF_POSITION_MASK, array_pos, mem_size);
	_update_vertex_element(fvf, D3DFVF_NORMAL, array_pos, mem_size);
	_update_vertex_element(fvf, D3DFVF_TEXCOUNT_MASK, array_pos, mem_size);
	_update_vertex_element(fvf, D3DFVF_DIFFUSE, array_pos, mem_size);
	_update_vertex_element(fvf, NULL, array_pos, mem_size);
}

bool GEVertexDecl::_update_vertex_element( DWORD fvf, DWORD add_fvf, int& array_pos, int& mem_offset )
{
	if (add_fvf != NULL && !(fvf & add_fvf)) return false;
	if (array_pos + 1 >= VERTEX_ELEMENT_MAX_CNT) return false;

#ifndef PUSH_VERTEX_ELEMENT
#define PUSH_VERTEX_ELEMENT(type, usage, usage_index, size) \
	_push_vertex_element(type, usage, usage_index, array_pos, mem_offset, size);
#endif PUSH_VERTEX_ELEMENT

	switch (add_fvf)
	{
	case D3DFVF_POSITION_MASK:
		{
			PUSH_VERTEX_ELEMENT(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0, (sizeof(float) * 3));

			UINT detail = (fvf & D3DFVF_POSITION_MASK);
			
			if (false) // not support
			{
				switch(detail)
				{
				case D3DFVF_XYZB1:
					break;
				case D3DFVF_XYZB2:
					PUSH_VERTEX_ELEMENT(D3DDECLTYPE_FLOAT1, D3DDECLUSAGE_BLENDWEIGHT, 0, (sizeof(float) * 1));
					break;
				case D3DFVF_XYZB3:
					PUSH_VERTEX_ELEMENT(D3DDECLTYPE_FLOAT2, D3DDECLUSAGE_BLENDWEIGHT, 0, (sizeof(float) * 2));
					break;
				case D3DFVF_XYZB4:
					PUSH_VERTEX_ELEMENT(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_BLENDWEIGHT, 0, (sizeof(float) * 3));
					break;
				case D3DFVF_XYZB5:
					PUSH_VERTEX_ELEMENT(D3DDECLTYPE_FLOAT4, D3DDECLUSAGE_BLENDWEIGHT, 0, (sizeof(float) * 4));
					break;
				}
			}


			if (detail >= D3DFVF_XYZB1 && detail <= D3DFVF_XYZB5)
			{
				if (0 != (fvf & D3DFVF_LASTBETA_UBYTE4))
				{
					PUSH_VERTEX_ELEMENT(D3DDECLTYPE_UBYTE4, D3DDECLUSAGE_BLENDINDICES, 0, (sizeof(UINT)));
				}
				else if (false && 0 != (fvf & D3DFVF_LASTBETA_D3DCOLOR)) // not support
				{
					PUSH_VERTEX_ELEMENT(D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_BLENDINDICES, 0, (sizeof(D3DCOLOR)));
				}
				else if (false && detail == D3DFVF_XYZB5) // not support
				{
					PUSH_VERTEX_ELEMENT(D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_BLENDINDICES, 0, (sizeof(D3DCOLOR)));
				}
			}

		}
		break;
	case D3DFVF_DIFFUSE:
		{
			PUSH_VERTEX_ELEMENT(D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_COLOR, 0, (sizeof(D3DCOLOR)));
		}
		break;
	case D3DFVF_NORMAL:
		{
			PUSH_VERTEX_ELEMENT(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_NORMAL, 0, (sizeof(float) * 3));
		}
		break;
	case D3DFVF_TEXCOUNT_MASK:
		{
			PUSH_VERTEX_ELEMENT(D3DDECLTYPE_FLOAT2, D3DDECLUSAGE_TEXCOORD,
				(BYTE(((add_fvf & fvf) >> D3DFVF_TEXCOUNT_SHIFT) - 1)), (sizeof(float) * 2));
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

bool GEVertexDecl::_push_vertex_element(
	BYTE type, BYTE usage, BYTE usage_index,
	int& array_pos, int& mem_offset, int elem_size )
{
	if (array_pos + 1 >= VERTEX_ELEMENT_MAX_CNT) return false;
	++ array_pos;

	vertex_element_[array_pos].Stream	= 0;
	vertex_element_[array_pos].Offset	= mem_offset;
	vertex_element_[array_pos].Method	= D3DDECLMETHOD_DEFAULT;
	vertex_element_[array_pos].Type		= type;
	vertex_element_[array_pos].Usage	= usage;
	vertex_element_[array_pos].UsageIndex = usage_index;
	mem_offset += elem_size;
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
, blend_(0)
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

void GE_VERTEX::set_blend( UINT blend )
{
	blend_ = blend;
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
	if (decl_->fvf & D3DFVF_POSITION_MASK)
	{
		_append_data(mem_buff, buff_offset, (void*)&(position_), sizeof(D3DXVECTOR3));

		DWORD detail = (decl_->fvf & D3DFVF_POSITION_MASK);
		if (detail >= D3DFVF_XYZB1 && detail <= D3DFVF_XYZB5)
		{
			if (0 != (decl_->fvf & D3DFVF_LASTBETA_UBYTE4))
			{
				_append_data(mem_buff, buff_offset, (void*)&(blend_), sizeof(UINT));
			}
		}
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


