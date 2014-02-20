#include "geo_atlas_render.h"
#include "../common/ge_engine.h"
#include "texture/ge_texture_manager.h"

namespace ge
{

DLL_MANAGE_CLASS_IMPLEMENT(GEOAtlasRender);

const DWORD GEOAtlasRender::DEFAULT_FVF_FORMAT = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

GEOAtlasRender::GEOAtlasRender()
: dx_vertex_buff_(NULL)
, dx_index_buff_(NULL)
, dx_quads_cnt_(0)
, vertex_decl_()
, texture_list_()
, quad_list_()
{
	set_vertex_fvf(DEFAULT_FVF_FORMAT);
}

GEOAtlasRender::~GEOAtlasRender()
{
	release();
}

bool GEOAtlasRender::init()
{
	return true;
}

void GEOAtlasRender::release()
{
	release_all_texture();
	release_render();

	vertex_decl_ = NULL;
}

bool GEOAtlasRender::set_vertex_fvf( WORD fvf )
{
	return set_vertex_decl(GEVertexDecl::get_vertex_decl(fvf));
}

bool GEOAtlasRender::set_vertex_decl( GE_VERTEX_DECL* vertex_decl )
{
	if (vertex_decl == NULL) return false;
	if (!vertex_decl->is_valid()) return false;

	vertex_decl_ = vertex_decl;

	release_render();
	return true;
}

GE_VERTEX_DECL* GEOAtlasRender::get_vertex_decl()
{
	return vertex_decl_;
}

int GEOAtlasRender::add_texture()
{
	GETexture* texture = GETextureManager::create_texture();
	texture_list_.push_back(texture);
	return (int)texture_list_.size() - 1;
}

int GEOAtlasRender::add_texture( const char* texture_path )
{
	GETexture* texture = GETextureManager::create_texture(texture_path);
	texture_list_.push_back(texture);
	return (int)texture_list_.size() - 1;
}

GETexture* GEOAtlasRender::get_texture( int texture_id/* = 0*/ )
{
	if (texture_id < 0) return NULL;
	if ((int)texture_list_.size() <= texture_id) return NULL;
	return texture_list_[texture_id];
}

bool GEOAtlasRender::replace_texture( int texture_id, const char* texture_path )
{
	if (texture_id < 0) return false;
	if ((int)texture_list_.size() >= texture_id) return false;
	release_texture(texture_id);
	texture_list_[texture_id] = GETextureManager::create_texture(texture_path);
	return true;
}

void GEOAtlasRender::release_texture( int texture_id )
{
	if (texture_id < 0) return;
	if ((int)texture_list_.size() >= texture_id) return;
	GETexture* texture = texture_list_[texture_id];
	if (texture)
	{
		GETextureManager::release_texture(texture);
		texture_list_[texture_id] = NULL;
	}
}

void GEOAtlasRender::release_all_texture()
{
	for (int i=0; i<(int)texture_list_.size(); ++i)
	{
		release_texture(i);
	}
	texture_list_.clear();
}

bool GEOAtlasRender::init_render()
{
	if (dx_quads_cnt_ < (int)quad_list_.size())
	{
		release_render();

		LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
		if (p_d3d_device == NULL) return false;

		if (vertex_decl_ == NULL) return false;
		if (!vertex_decl_->is_valid()) return false;

		int quads_cnt = (int)quad_list_.size();
		
		HRESULT h_res = S_OK;
		std::vector<WORD> indices;
		indices.resize(quads_cnt * 6);

		h_res = p_d3d_device->CreateVertexBuffer(
			quads_cnt * 4 * vertex_decl_->size,
			D3DUSAGE_WRITEONLY,
			NULL,
			D3DPOOL_MANAGED,
			&dx_vertex_buff_,
			NULL);
		if (FAILED(h_res)) goto init_faild;

		h_res = p_d3d_device->CreateIndexBuffer(
			quads_cnt * 6 * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&dx_index_buff_,
			NULL);		
		if (FAILED(h_res)) goto init_faild;
		dx_quads_cnt_ = quads_cnt;

		for (int i=0; i<quads_cnt; ++i)
		{
			int i6 = i * 6;
			int i4 = i * 4;
			indices[i6 + 0] = i4 + 0;
			indices[i6 + 1] = i4 + 1;
			indices[i6 + 2] = i4 + 3;
			indices[i6 + 3] = i4 + 1;
			indices[i6 + 4] = i4 + 2;
			indices[i6 + 5] = i4 + 3;
		}
		if(!_set_indices(indices)) goto init_faild;
		
		return true;

init_faild:
		release_render();
		return false;

	} else return true;
}

bool GEOAtlasRender::update_render()
{
	std::vector<GE_VERTEX> verties;
	verties.resize(quad_list_.size() * 4);

	int offset = 0;
	FOR_EACH (QUAD_LIST, quad_list_, quad)
	{
		verties[offset + 0] = quad->tl;
		verties[offset + 1] = quad->tr;
		verties[offset + 2] = quad->br;
		verties[offset + 3] = quad->bl;
		offset += 4;
	}

	return _set_verties(verties);
}

bool GEOAtlasRender::update_render_task()
{
	render_task_list_.clear();

	int offset = 0;
	int current_task = -1;
	int current_texture = -1;
	FOR_EACH (QUAD_LIST, quad_list_, quad)
	{
		if(quad->texture != current_texture)
		{
			current_texture = quad->texture;
			current_task = (int) render_task_list_.size();

			QUAD_RENDER_TASK render_task;
			render_task.offset = offset;
			render_task.count = 1;
			render_task.texture = quad->texture;
			render_task_list_.push_back(render_task);
		}
		else
		{
			++ render_task_list_[current_task].count;
		}
		++ offset;
	}
	return true;
}

bool GEOAtlasRender::_set_verties( std::vector<GE_VERTEX>& vertex_array )
{
	if (dx_vertex_buff_ == NULL) return false;

	if (vertex_decl_ == NULL) return false;
	if (!vertex_decl_->is_valid()) return false;

	int vertex_cnt = (int)vertex_array.size();
	if ( vertex_cnt > dx_quads_cnt_ * 4) return false;

	char* vertex_buff = NULL;
	HRESULT h_res = dx_vertex_buff_->Lock(
		0, vertex_cnt * vertex_decl_->size,
		(void**)&vertex_buff, 0);

	for (int i=0; i < vertex_cnt; ++i)
	{
		bool b_ret = true;
		b_ret = vertex_array[i].pack(vertex_buff, vertex_decl_->size);
		if (!b_ret)
		{
			b_ret = false;
			break;
		}

		vertex_buff += vertex_decl_->size;
	}

	dx_vertex_buff_->Unlock();
	return true;
}

bool GEOAtlasRender::_set_indices( std::vector<WORD>& index_array )
{
	int index_cnt = (int)index_array.size();
	if ( index_cnt > dx_quads_cnt_ * 6) return false;

	WORD* index_buff = NULL;
	dx_index_buff_->Lock(
		0, index_cnt * sizeof(WORD),
		(void**)&index_buff, 0);
	for (int i=0; i < index_cnt; ++i)
	{
		index_buff[i] = index_array[i];
	}
	dx_index_buff_->Unlock();
	return true;
}

void GEOAtlasRender::release_render()
{
	D3D_RELEASE(dx_vertex_buff_);
	D3D_RELEASE(dx_index_buff_);
	dx_quads_cnt_ = 0;
	task_list_need_update_ = true;
}

bool GEOAtlasRender::add_quad( GE_QUAD& quad )
{
	quad_list_.push_back(quad);
	task_list_need_update_ = true;
	return true;
}

void GEOAtlasRender::clear_quads()
{
	quad_list_.clear();
	task_list_need_update_ = true;
}

bool GEOAtlasRender::merge_quads()
{
	return true;
}

bool GEOAtlasRender::draw_quads()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	if (vertex_decl_ == NULL) return false;
	if (!vertex_decl_->is_valid()) return false;

	HRESULT h_res = S_OK;
	h_res = p_d3d_device->SetStreamSource(0, dx_vertex_buff_, 0, vertex_decl_->size);
	assert(SUCCEEDED(h_res));
	h_res = p_d3d_device->SetIndices(dx_index_buff_);
	assert(SUCCEEDED(h_res));
	h_res = p_d3d_device->SetVertexDeclaration(vertex_decl_->decl);
	assert(SUCCEEDED(h_res));

	FOR_EACH (QUAD_RENDER_TASK_LIST, render_task_list_, task)
	{
		GETexture* texture = get_texture(task->texture);
		if(texture) texture->use_texture();
		else GETexture::use_null_texture();

		h_res = p_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,						// BaseVertexIndex
			0,						// MinVertexIndex
			dx_quads_cnt_ * 4,		// NumVertices
			task->offset * 6,		// StartIndex
			task->count * 2);		// PrimitiveCount
		assert(SUCCEEDED(h_res));
	}
	GETexture::use_null_texture();
	return true;
}

void GEOAtlasRender::render( time_t delta )
{
	if (task_list_need_update_)
	{
		if (!init_render()) return;
		if (!update_render()) return;
		if (!update_render_task()) return;
		task_list_need_update_ = false;
	}

	draw_quads();
}


}