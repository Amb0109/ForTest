#include "geu_vertex.h"
#include "../common/ge_engine.h"

namespace ge
{

GE_VERTEX::GE_VERTEX()
:position_(0.f, 0.f, 0.f), normal_(0.f, 0.f, 0.f),
texcoords_(0.f, 0.f), color_(0xffffffff),
vertex_size_(0), p_vertex_decl_(NULL)
{
	this->set_fvf(NULL);
	memset(vertex_buff_, 0, sizeof(vertex_buff_));
}

bool GE_VERTEX::set_fvf( DWORD fvf )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	fvf_ = fvf;
	
	int element_array_pos = 0;
	int vertex_buff_size = 0;
	_calc_vertex_element_array(element_array_pos, vertex_buff_size);
	vertex_size_ = vertex_buff_size;

	HRESULT h_res = S_OK;
	SAFE_RELEASE(p_vertex_decl_);
	h_res = p_d3d_device->CreateVertexDeclaration(vertex_element_, &p_vertex_decl_);

	assert(SUCCEEDED(h_res));
	return SUCCEEDED(h_res);
}

const void* GE_VERTEX::pack()
{
	int mem_size = 0;
	memset(vertex_buff_, 0, sizeof(vertex_buff_));
	if (fvf_ & D3DFVF_XYZ)
	{
		_append_data(mem_size, (void*)&(position_.x), sizeof(float));
		_append_data(mem_size, (void*)&(position_.y), sizeof(float));
		_append_data(mem_size, (void*)&(position_.z), sizeof(float));
	}

	if (fvf_ & D3DFVF_NORMAL)
	{
		_append_data(mem_size, (void*)&(normal_.x), sizeof(float));
		_append_data(mem_size, (void*)&(normal_.y), sizeof(float));
		_append_data(mem_size, (void*)&(normal_.z), sizeof(float));
	}

	if (fvf_ & D3DFVF_TEXCOORDSIZE1(0))
	{
		_append_data(mem_size, (void*)&(texcoords_.x), sizeof(float));
		_append_data(mem_size, (void*)&(texcoords_.y), sizeof(float));
	}

	if (fvf_ & D3DFVF_DIFFUSE)
	{
		_append_data(mem_size, (void*)&(color_), sizeof(D3DCOLOR));
	}
	assert(mem_size == vertex_size_);
	if (mem_size != vertex_size_) return NULL;
	return vertex_buff_;
}

bool GE_VERTEX::_append_data( int& mem_offset, void* p_data, int data_size )
{
	if (mem_offset + data_size > VERTEX_BUFF_MAX_SIZE) return false;
	memcpy(vertex_buff_ + mem_offset, p_data, data_size);
	mem_offset += data_size;
	return true;
}

void GE_VERTEX::_calc_vertex_element_array( int& array_pos, int& mem_size )
{
	array_pos = -1;
	mem_size = 0;

	ZeroMemory(vertex_element_, sizeof(vertex_element_));
	_add_vertex_element(D3DFVF_XYZ, array_pos, mem_size);
	_add_vertex_element(D3DFVF_NORMAL, array_pos, mem_size);
	_add_vertex_element(D3DFVF_TEXCOORDSIZE1(0), array_pos, mem_size);
	_add_vertex_element(D3DFVF_DIFFUSE, array_pos, mem_size);
	_add_vertex_element(NULL, array_pos, mem_size);
}

bool GE_VERTEX::_add_vertex_element( DWORD fvf_type, int& array_pos, int& mem_offset )
{
	if (fvf_type != NULL && !(fvf_ & fvf_type)) return false;
	if (array_pos + 1 >= VERTEX_ELEMENT_MAX_CNT) return false;

	switch (fvf_type)
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
	case D3DFVF_TEXCOORDSIZE1(0):
		{
			++ array_pos;
			vertex_element_[array_pos].Stream	= 0;
			vertex_element_[array_pos].Offset	= mem_offset;
			vertex_element_[array_pos].Method	= D3DDECLMETHOD_DEFAULT;
			vertex_element_[array_pos].Type		= D3DDECLTYPE_FLOAT2;
			vertex_element_[array_pos].Usage	= D3DDECLUSAGE_TEXCOORD;
			vertex_element_[array_pos].UsageIndex = 0;
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

}


