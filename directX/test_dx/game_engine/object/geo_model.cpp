#include "../common/ge_app.h"
#include "../common/ge_engine.h"
#include "../utility/geu_gmath.h"
#include "geo_model.h"

namespace ge
{

GEOModel::GEOModel()
:d3d_vertex_buff_(NULL),
d3d_index_buff_(NULL),
vertex_buff_size_(0),
index_buff_size_(0)
{
	GEObject::type_ = GEObjectType_Model;
}

GEOModel::~GEOModel()
{

}

bool GEOModel::create_vetrix_buff( int buff_size )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	release_vetrix_buff();
	if (buff_size <= 0) return true;

	HRESULT h_res = p_d3d_device->CreateVertexBuffer(
		buff_size * sizeof(GE_VERTEX),
		D3DUSAGE_WRITEONLY,
		GE_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&d3d_vertex_buff_,
		NULL
		);

	vertex_buff_size_ = buff_size;
	return SUCCEEDED(h_res);
}

bool GEOModel::create_index_buff( int buff_size )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	release_index_buff();
	if (buff_size <= 0) return true;

	HRESULT h_res = p_d3d_device->CreateIndexBuffer(
		buff_size * sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&d3d_index_buff_,
		NULL
		);

	index_buff_size_ = buff_size;
	return SUCCEEDED(h_res);
}

bool GEOModel::set_vetrix( int id, GE_VERTEX& vertex )
{
	if(d3d_vertex_buff_ == NULL) return false;
	if(id < 0 || id >= vertex_buff_size_) return false;

	GE_VERTEX* vertex_buff = NULL;
	d3d_vertex_buff_->Lock(id, 1, (void**)&vertex_buff, 0);
	vertex_buff[id] = vertex;
	d3d_vertex_buff_->Unlock();
	return true;
}

bool GEOModel::set_index( int id, WORD index )
{
	if(d3d_index_buff_ == NULL) return false;
	if(id < 0 || id >= index_buff_size_) return false;

	WORD* index_buff = NULL;
	d3d_index_buff_->Lock(id, 1, (void**)&index_buff, 0);
	index_buff[id] = index;
	d3d_index_buff_->Unlock();
	return true;
}

bool GEOModel::set_vertices( GE_VERTEX* vertex_array, int size )
{
	if(!create_vetrix_buff(size))
		return false;

	GE_VERTEX* vertex_buff = NULL;
	d3d_vertex_buff_->Lock(0, 0, (void**)&vertex_buff, 0);
	for (int i=0; i < size; ++i)
	{
		vertex_buff[i] = vertex_array[i];
	}
	d3d_vertex_buff_->Unlock();
	return true;
}

bool GEOModel::set_indices( WORD* index_array, int size )
{
	if(!create_index_buff(size))
		return false;

	WORD* index_buff = NULL;
	d3d_index_buff_->Lock(0, 0, (void**)&index_buff, 0);
	for (int i=0; i < size; ++i)
	{
		index_buff[i] = index_array[i];
	}
	d3d_index_buff_->Unlock();
	return true;
}

void GEOModel::release_vetrix_buff()
{
	SAFE_RELEASE(d3d_vertex_buff_);
	vertex_buff_size_ = 0;
}

void GEOModel::release_index_buff()
{
	SAFE_RELEASE(d3d_index_buff_);
	index_buff_size_ = 0;
}

bool GEOModel::init()
{
	return true;
}

void GEOModel::destory()
{
	release_vetrix_buff();
	release_index_buff();
}

void GEOModel::update( time_t time_elapsed )
{

}

void GEOModel::render( time_t time_elapsed )
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return;
	if (d3d_vertex_buff_ == NULL) return;
	if (d3d_index_buff_ == NULL) return;

	HRESULT h_res = S_OK;
	h_res = p_d3d_device->SetStreamSource(0, d3d_vertex_buff_, 0, sizeof(GE_VERTEX));
	h_res = p_d3d_device->SetIndices(d3d_index_buff_);
	h_res = p_d3d_device->SetFVF(GE_VERTEX::FVF);
	h_res = p_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,						// BaseVertexIndex
		0,						// MinVertexIndex
		vertex_buff_size_,		// NumVertices
		0,						// StartIndex
		index_buff_size_ / 3);	// PrimitiveCount
}

}