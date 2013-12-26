#include "geo_spine.h"
#include "../utility/geu_gmath.h"
#include "../common/ge_app.h"
#include "../common/ge_input.h"

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
	ge::GEAtlasPageManager::get_instence()->create_texture(self, path);
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	ge::GEAtlasPageManager::get_instence()->dispose_texture(self);
}

char* _spUtil_readFile (const char* path, int* length) {
	return _readFile(path, length);
}

ge::GEAtlasPageManager* ge::GEAtlasPageManager::get_instence()
{
	static GEAtlasPageManager _g_atlas_page_manager;
	return &_g_atlas_page_manager;
}

bool ge::GEAtlasPageManager::create_texture( spAtlasPage* atlas_page, const char* texture_path )
{
	if (texture_map_.find(atlas_page) != texture_map_.end()) return true;
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	LPDIRECT3DTEXTURE9 p_img_texture = NULL;
	HRESULT h_res = S_OK;
	h_res = D3DXCreateTextureFromFile(p_d3d_device, texture_path, &p_img_texture);
	if (FAILED(h_res)) return false;
	texture_map_[atlas_page] = p_img_texture;

	D3DSURFACE_DESC surface_desc;
	p_img_texture->GetLevelDesc(0, &surface_desc);
	atlas_page->width = surface_desc.Width;
	atlas_page->height = surface_desc.Height;
	atlas_page->rendererObject = NULL;

	return true;
}

LPDIRECT3DTEXTURE9 ge::GEAtlasPageManager::get_texture( spAtlasPage* atlas_page )
{
	if (texture_map_.find(atlas_page) == texture_map_.end()) return NULL;
	return texture_map_[atlas_page];
}

void ge::GEAtlasPageManager::dispose_texture( spAtlasPage* atlas_page )
{
	if (texture_map_.find(atlas_page) == texture_map_.end()) return;
	texture_map_[atlas_page]->Release();
	texture_map_.erase(atlas_page);
}





ge::GEOSpine::GEOSpine()
:p_atlas_(NULL),
p_json_(NULL),
p_skeleton_data_(NULL),
p_skeleton_(NULL),
p_animation_(NULL),
draw_bone_mesh_(true)
{

}

ge::GEOSpine::~GEOSpine()
{

}

bool ge::GEOSpine::init()
{
	p_atlas_ = spAtlas_readAtlasFile("texture/spineboy.atlas");
	p_json_ = spSkeletonJson_create(p_atlas_);
	p_skeleton_data_ = spSkeletonJson_readSkeletonDataFile(p_json_, "texture/spineboy.json");
	if (!p_skeleton_data_) return false;

	p_skeleton_ = spSkeleton_create(p_skeleton_data_);
	//spSkeleton_setSkinByName(p_skeleton_, "goblingirl");
	spSkeleton_setToSetupPose(p_skeleton_);

	spAnimationStateData* p_animation_state_data = spAnimationStateData_create(p_skeleton_data_);
	p_animation_state_ = spAnimationState_create(p_animation_state_data);

	_init_draw_panel();
	_init_bone_mesh();
	set_animation("walk");
	
	transform_.py = -100;
	_calc_world_matrix();

	return true;
}

void ge::GEOSpine::destory()
{
	spSkeleton_dispose(p_skeleton_);
	spSkeletonData_dispose(p_skeleton_data_);
	spSkeletonJson_dispose(p_json_);
	spAtlas_dispose(p_atlas_);
}

void ge::GEOSpine::update( time_t time_elapsed )
{
	GEInput* p_input = GEApp::get_instance()->get_input();
	if (p_input)
	{
		if (p_input->get_key_down(DIK_J))
			set_animation("jump");
		else if (p_input->get_key_down(DIK_W))
			set_animation("walk");
		
		if (p_input->get_key_down(DIK_D))
			draw_bone_mesh_ = !draw_bone_mesh_;
	}

	if (p_skeleton_ == NULL) return;
	spAnimationState_update(p_animation_state_, time_elapsed / 1000.f);
	spSkeleton_update(p_skeleton_, time_elapsed / 1000.f);
	spAnimationState_apply(p_animation_state_, p_skeleton_);
}

void ge::GEOSpine::render( time_t time_elapsed )
{
	if (p_skeleton_ == NULL) return;
	spSkeleton_updateWorldTransform(p_skeleton_);

	int slot_cnt = p_skeleton_->slotCount;
	for (int i=0; i<slot_cnt; ++i)
	{
		spSlot* slot = p_skeleton_->slots[i];
		spAttachment* attachment = slot->attachment;
		if (attachment == NULL) continue;
		if (attachment && attachment->type ==  ATTACHMENT_REGION)
		{
			spRegionAttachment* region_attachment = (spRegionAttachment*)attachment;
			if (region_attachment == NULL) continue;
			spAtlasRegion* atlas_region = (spAtlasRegion*)region_attachment->rendererObject;

			if (_load_region_texture(atlas_region))
			{
				const spBone* bone = slot->bone;
				_transform_region_texture(region_attachment, bone);
				_do_slot_render();
			}
		}
	}

	if (draw_bone_mesh_) _render_bone();
}

bool ge::GEOSpine::set_animation( const char* state )
{
	if (p_skeleton_data_ == NULL) return false;

	p_animation_ = spSkeletonData_findAnimation(p_skeleton_data_, state);
	if (p_animation_)
	{
		spAnimationState_setAnimation(p_animation_state_, 0, p_animation_, true);
		return true;
	}
	return false;
}

bool ge::GEOSpine::_init_draw_panel()
{
	ge::GE_VERTEX_DECL	vertex_decl_;
	vertex_decl_.init(DEF_FVF_FORMAT);
	set_vertex_decl(&vertex_decl_);

	WORD index_buff[6];
	index_buff[0] = 0; index_buff[1] = 3; index_buff[2] = 1;
	index_buff[3] = 3; index_buff[4] = 2; index_buff[5] = 1;
	set_indices(index_buff, 6);

	return true;
}

bool ge::GEOSpine::_load_region_texture(const spAtlasRegion* atlas_region )
{
	if(atlas_region == NULL) return false;

	ge::GE_VERTEX_DECL	vertex_decl_;
	vertex_decl_.init(DEF_FVF_FORMAT);
	set_vertex_decl(&vertex_decl_);

	float width = (float)atlas_region->width;
	float height = (float)atlas_region->height;

	ge::GE_VERTEX vertex_buff[4];
	for (int i=0; i<4; ++i) vertex_buff[i].set_decl(&vertex_decl_);

	vertex_buff[0].set_position(D3DXVECTOR3(0.f, 0.f, 0.f));
	vertex_buff[0].set_texcoords(D3DXVECTOR2(atlas_region->u, atlas_region->v));

	vertex_buff[1].set_position(D3DXVECTOR3(0.f, -height, 0.f));
	vertex_buff[1].set_texcoords(D3DXVECTOR2(atlas_region->u, atlas_region->v2));

	vertex_buff[2].set_position(D3DXVECTOR3(width, -height, 0.f));
	vertex_buff[2].set_texcoords(D3DXVECTOR2(atlas_region->u2, atlas_region->v2));

	vertex_buff[3].set_position(D3DXVECTOR3(width, 0.f, 0.f));
	vertex_buff[3].set_texcoords(D3DXVECTOR2(atlas_region->u2, atlas_region->v));
	set_vertices(vertex_buff, 4);

	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	LPDIRECT3DTEXTURE9 p_texture = GEAtlasPageManager::get_instence()->get_texture(p_atlas_->pages);
	p_d3d_device->SetTexture(0, p_texture);

	return true;
}

bool ge::GEOSpine::_transform_region_texture(const spRegionAttachment* region_attachment, const spBone* bone)
{
	if (region_attachment == NULL) return false;
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	D3DXMATRIX trans_to_center_matrix;
	D3DXMATRIX org_trans_matrix;
	D3DXMATRIX org_rotate_matrix;
	D3DXMATRIX org_scale_matrix;
	D3DXMATRIX trans_matrix;
	D3DXMATRIX rotate_matrix;
	D3DXMATRIX scale_matrix;

	// 中点挪到0,0
	D3DXMatrixTranslation(&trans_to_center_matrix, - region_attachment->regionWidth / 2.f, region_attachment->regionHeight / 2.f, 0.f);
	// 相对骨骼变换
	D3DXMatrixRotationZ(&org_rotate_matrix, region_attachment->rotation / 180.f * 3.141592654f);
	D3DXMatrixScaling(&org_scale_matrix, region_attachment->scaleX, region_attachment->scaleY, 1.0f);
	D3DXMatrixTranslation(&org_trans_matrix, region_attachment->x, region_attachment->y, 0.f);
	// 骨骼变换
	D3DXMatrixRotationZ(&rotate_matrix, bone->worldRotation / 180.f * 3.141592654f);
	D3DXMatrixScaling(&scale_matrix, bone->worldScaleX, bone->worldScaleY, 1.0f);
	D3DXMatrixTranslation(&trans_matrix, bone->worldX, bone->worldY, 0.f);

	D3DXMATRIX world_matrix;
	world_matrix = trans_to_center_matrix;
	world_matrix = world_matrix * org_scale_matrix;
	world_matrix = world_matrix * org_rotate_matrix;
	world_matrix = world_matrix * org_trans_matrix;
	world_matrix = world_matrix * scale_matrix;
	world_matrix = world_matrix * rotate_matrix;
	world_matrix = world_matrix * trans_matrix;
	world_matrix = world_matrix * world_matrix_; //最终整体的变换

	p_d3d_device->SetTransform(D3DTS_WORLD, &world_matrix);

	return true;
}

bool ge::GEOSpine::_do_slot_render()
{
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;

	if (d3d_vertex_buff_ == NULL) return true;
	if (d3d_index_buff_ == NULL) return true;

	if (p_d3d_decl_ == NULL) return true;
	if (vertex_size_ <= 0) return true;

	HRESULT h_res = S_OK;
	h_res = p_d3d_device->SetStreamSource(0, d3d_vertex_buff_, 0, vertex_size_);
	h_res = p_d3d_device->SetIndices(d3d_index_buff_);
	h_res = p_d3d_device->SetVertexDeclaration(p_d3d_decl_);
	h_res = p_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,						// BaseVertexIndex
		0,						// MinVertexIndex
		vertex_cnt_,			// NumVertices
		0,						// StartIndex
		index_cnt_ / 3);		// PrimitiveCount
	return true;
}

bool ge::GEOSpine::_init_bone_mesh()
{
	ge::GE_VERTEX_DECL	vertex_decl_;
	vertex_decl_.init(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	bone_mesh_.set_vertex_decl(&vertex_decl_);

	ge::GE_VERTEX vertex_buff[3];
	for (int i=0; i<3; ++i) vertex_buff[i].set_decl(&vertex_decl_);

	vertex_buff[0].set_position(D3DXVECTOR3(0.f, 3.f, 0.f));
	vertex_buff[0].set_color(0xffffffff);

	vertex_buff[1].set_position(D3DXVECTOR3(0.f, -3.f, 0.f));
	vertex_buff[1].set_color(0xffffffff);

	vertex_buff[2].set_position(D3DXVECTOR3(100.f, 0.0f, 0.f));
	vertex_buff[2].set_color(0xffffffff);
	bone_mesh_.set_vertices(vertex_buff, 3);

	WORD index_buff[3];
	index_buff[0] = 0; index_buff[1] = 2; index_buff[2] = 1;
	bone_mesh_.set_indices(index_buff, 3);
	return true;
}

bool ge::GEOSpine::_render_bone()
{
	if (p_skeleton_ == NULL) return false;
	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return false;
	
	int slot_cnt = p_skeleton_->slotCount;

	D3DXMATRIX trans_matrix;
	D3DXMATRIX rotate_matrix;
	D3DXMATRIX scale_matrix;
	D3DXMATRIX world_matrix;

	p_d3d_device->SetTexture(0, NULL);
	for (int i=0; i<slot_cnt; ++i)
	{
		spSlot* slot = p_skeleton_->slots[i];
		const spBone* bone = slot->bone;

		D3DXMatrixTranslation(&trans_matrix, bone->worldX, bone->worldY, -1.f);
		D3DXMatrixRotationZ(&rotate_matrix, bone->worldRotation / 180.f * 3.141592654f);
		D3DXMatrixScaling(&scale_matrix, bone->data->length / 100.f, 1.f, 1.0f);

		world_matrix = scale_matrix * rotate_matrix * trans_matrix * world_matrix_;

		p_d3d_device->SetTransform(D3DTS_WORLD, &world_matrix);
		bone_mesh_.render(0);
	}
	return true;
}

