#include "geo_spine.h"
#include "../utility/geu_gmath.h"
#include "../common/ge_app.h"
#include "../common/ge_input.h"

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
	if (self == NULL) return;
	ge::GEOPrimitive* render_object = ge::GEOPrimitive::create();
	if (render_object == NULL) return;
	render_object->create_texture(path);
	render_object->get_texture_size(self->width, self->height);
	self->rendererObject = (void*)render_object;
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	ge::GEOPrimitive* render_object = (ge::GEOPrimitive*)(self->rendererObject);
	if (render_object == NULL) return;
	ge::GEOPrimitive::release(&render_object);
}

char* _spUtil_readFile (const char* path, int* length) {
	return _readFile(path, length);
}

namespace ge {

DLL_MANAGE_CLASS_IMPLEMENT(GEOSpine);

GEOSpine::GEOSpine()
:p_atlas_(NULL),
p_skeleton_json_(NULL),
p_skeleton_data_(NULL),
p_skeleton_(NULL),
p_animation_(NULL),
p_animation_state_(NULL),
p_animation_state_data_(NULL),
draw_bone_mesh_(true)
{

}

GEOSpine::~GEOSpine()
{
	destory();
}

bool GEOSpine::init()
{
	p_atlas_ = spAtlas_readAtlasFile("texture/spineboy.atlas");
	p_skeleton_json_ = spSkeletonJson_create(p_atlas_);
	p_skeleton_data_ = spSkeletonJson_readSkeletonDataFile(p_skeleton_json_, "texture/spineboy.json");
	if (!p_skeleton_data_) return false;

	p_skeleton_ = spSkeleton_create(p_skeleton_data_);
	//spSkeleton_setSkinByName(p_skeleton_, "goblingirl");
	spSkeleton_setToSetupPose(p_skeleton_);

	p_animation_state_data_ = spAnimationStateData_create(p_skeleton_data_);
	spAnimationStateData_setMixByName(p_animation_state_data_, "walk", "jump", 0.3f);
	spAnimationStateData_setMixByName(p_animation_state_data_, "jump", "walk", 0.3f);
	spAnimationStateData_setMixByName(p_animation_state_data_, "jump", "jump", 0.3f);

	p_animation_state_ = spAnimationState_create(p_animation_state_data_);
	spAnimationState_setAnimationByName(p_animation_state_, 0, "walk", true);

	vertex_decl_.init(D3DFVF_XYZ | D3DFVF_TEX1);
	bone_vertex_decl_.init(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	
	transform_.py = -100;

	return true;
}

void GEOSpine::destory()
{
	if(p_animation_state_)		spAnimationState_dispose(p_animation_state_);
	if(p_animation_state_data_)	spAnimationStateData_dispose(p_animation_state_data_);
	if(p_skeleton_)				spSkeleton_dispose(p_skeleton_);
	if(p_skeleton_data_)		spSkeletonData_dispose(p_skeleton_data_);
	if(p_skeleton_json_)		spSkeletonJson_dispose(p_skeleton_json_);
	if(p_atlas_)				spAtlas_dispose(p_atlas_);
}

void GEOSpine::update( time_t delta )
{
	GEInput* p_input = GEApp::get_instance()->get_input();
	if (p_input)
	{
		if (p_input->get_key_down(DIK_J))
		{		
			spAnimationState_addAnimationByName(p_animation_state_, 0, "jump", false, 0);
			spAnimationState_addAnimationByName(p_animation_state_, 0, "walk", true, 0);
		}
		if (p_input->get_key_down(DIK_D))
			draw_bone_mesh_ = !draw_bone_mesh_;
	}

	if (p_skeleton_ == NULL) return;
	spSkeleton_update(p_skeleton_, delta / 1000.f);
	spAnimationState_update(p_animation_state_, delta / 1000.f);
	spAnimationState_apply(p_animation_state_, p_skeleton_);
}

void GEOSpine::render( time_t delta )
{
	if (p_skeleton_ == NULL) return;
	spSkeleton_updateWorldTransform(p_skeleton_);

	LPDIRECT3DDEVICE9 p_d3d_device = GEEngine::get_instance()->get_device();
	if (p_d3d_device == NULL) return;
	p_d3d_device->SetTransform(D3DTS_WORLD, &get_world_transform());

	_do_render();
	if (draw_bone_mesh_)
		_do_bone_render();
}

void GEOSpine::_do_render()
{
	if (p_atlas_ == NULL) return;
	GEOPrimitive* render_object = (GEOPrimitive*)p_atlas_->pages->rendererObject;
	if (render_object == NULL) return;

	if (p_skeleton_ == NULL) return;

	int slot_cnt = p_skeleton_->slotCount;

	std::vector<GE_VERTEX> vertex_buff;
	std::vector<WORD> index_buff;
	vertex_buff.resize(slot_cnt * 4);
	index_buff.resize(slot_cnt * 6);
	index_buff[0] = 0; index_buff[1] = 1; index_buff[2] = 3;
	index_buff[3] = 3; index_buff[4] = 1; index_buff[5] = 2;

	int region_offset = 0;
	for (int i=0; i<slot_cnt; ++i)
	{
		spSlot* slot = p_skeleton_->slots[i];
		spAttachment* attachment = slot->attachment;
		if (attachment && attachment->type ==  ATTACHMENT_REGION)
		{
			spRegionAttachment* region_attachment = (spRegionAttachment*)attachment;
			if (region_attachment == NULL) continue;

			float verties[8], *uvs;
			spRegionAttachment_computeWorldVertices(region_attachment, 0.f, 0.f, slot->bone, verties);
			uvs = region_attachment->uvs;
			
			int vertex_offset = 4 * region_offset;
			for (int ii=0; ii<4; ++ii)
			{
				GE_VERTEX vertex;
				vertex.set_decl(&vertex_decl_);
				vertex.set_position(D3DXVECTOR3(verties[ii<<1], verties[ii<<1|1], 0.f));
				vertex.set_texcoords(D3DXVECTOR2(uvs[ii<<1], uvs[ii<<1|1]));
				vertex_buff[vertex_offset + ii] = vertex;
			}

			if (region_offset != 0)
			{
				int index_offset = 6 * region_offset;
				for (int ii=0; ii<6; ++ii)
				{
					index_buff[index_offset + ii] = index_buff[ii + index_offset - 6] + 4;
				}
			}
			++ region_offset;
		}
	}

	if (render_object->get_vertex_buff_size() <= 0)
	{
		render_object->set_vertex_decl(&vertex_decl_);
		render_object->create_vetrix_buff(slot_cnt * 4);
		render_object->create_index_buff(slot_cnt * 6);
	}

	render_object->set_vertices(&vertex_buff[0], 0, region_offset * 4);
	render_object->set_indices(&index_buff[0], 0, region_offset * 6);
	render_object->set_primitive_draw(0, region_offset * 2);
	render_object->render(0);
}

void GEOSpine::_do_bone_render()
{
	if (p_skeleton_ == NULL) return;

	int slot_cnt = p_skeleton_->slotCount;

	std::vector<GE_VERTEX> vertex_buff;
	vertex_buff.resize(slot_cnt * 3);

	int render_bone_cnt = 0;
	for (int i=0; i<slot_cnt; ++i)
	{
		spSlot* slot = p_skeleton_->slots[i];
		if (slot == NULL) continue;

		const spBone* bone = slot->bone;
		if (bone == NULL) continue;

		const spBoneData* bone_data = bone->data;
		if (bone_data == NULL) continue;

		if (bone_data->length - FLT_EPSILON < 0.f) continue;

		float plx = 0.f, ply = -5.f;
		float prx = 0.f, pry = 5.f;
		float ptx = bone->data->length, pty = 0.f;

		float fangle = bone->worldRotation / 180.f * 3.141592654f;
		float fsin = sin(fangle);
		float fcos = cos(fangle);

		float vlx = plx * fcos - ply * fsin + bone->worldX;
		float vly = plx * fsin + ply * fcos + bone->worldY;
		float vrx = prx * fcos - pry * fsin + bone->worldX;
		float vry = prx * fsin + pry * fcos + bone->worldY;
		float vtx = ptx * fcos - pty * fsin + bone->worldX;
		float vty = ptx * fsin + pty * fcos + bone->worldY;

		int vertex_offset = render_bone_cnt * 3;
		GE_VERTEX vertex;
		vertex.set_decl(&bone_vertex_decl_);
		vertex.set_color(D3DXCOLOR(0xffffffff));

		vertex.set_position(D3DXVECTOR3(vlx, vly, 0.f));
		vertex_buff[vertex_offset + 0] = vertex;

		vertex.set_position(D3DXVECTOR3(vrx, vry, 0.f));
		vertex_buff[vertex_offset + 1] = vertex;

		vertex.set_position(D3DXVECTOR3(vtx, vty, 0.f));
		vertex_buff[vertex_offset + 2] = vertex;

		++ render_bone_cnt;
	}

	if (bone_mesh_.get_vertex_buff_size() <= 0)
	{
		bone_mesh_.set_vertex_decl(&bone_vertex_decl_);
		bone_mesh_.create_vetrix_buff(slot_cnt * 3);
	}

	bone_mesh_.set_vertices(&vertex_buff[0], 0, render_bone_cnt * 3);
	bone_mesh_.set_primitive_draw(0, render_bone_cnt);
	bone_mesh_.render(0);
}
}
