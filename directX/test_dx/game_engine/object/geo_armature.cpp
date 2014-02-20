#include "geo_armature.h"

namespace CC
{

const unsigned CC_VERTEX_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

bool CCTextureAtlas::init(CCAtlasPage* atlasPage)
{
	if (atlasPage == NULL) return false;
	return initTexture(atlasPage->m_sImageFile.c_str());
}

void CCTextureAtlas::release()
{
	releaseTexture();
}

void CCTextureAtlas::drawQuads()
{
	ge::GEOPrimitive* render_object = (ge::GEOPrimitive*)m_pRenderObject;
	if (render_object == NULL) return;

	int verties_cnt = (int)m_vecPaintVerties.size();
	if (verties_cnt <= 0) return;

	if (verties_cnt > render_object->get_vertex_buff_size())
	{
		render_object->create_vetrix_buff(verties_cnt);
		render_object->create_index_buff(verties_cnt / 4 * 6);
		
		WORD indices[6];
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 3; indices[4] = 0; indices[5] = 2;
		for (int i=0; i < verties_cnt/4; i++)
		{
			render_object->set_indices(indices, i*6, 6);
			for (int j=0; j < 6; j++) indices[j] += 4;
		}
	}

	std::vector<ge::GE_VERTEX> verties_list;
	verties_list.reserve(verties_cnt);

	FOR_EACH (std::vector<CC_PAINT_VERTEX>, m_vecPaintVerties, it)
	{
		ge::GE_VERTEX vertex;
		vertex.set_decl(render_object->get_vertex_decl());
		vertex.set_position(D3DXVECTOR3(it->x, it->y, it->z));
		vertex.set_color(D3DCOLOR(it->color));
		vertex.set_texcoords(D3DXVECTOR2(it->u, it->v));
		verties_list.push_back(vertex);
	}

	render_object->set_vertices(&verties_list[0], 0, verties_cnt);
	render_object->set_primitive_draw(0, verties_cnt / 2);
	render_object->render(0);
}

bool CCTextureAtlas::initTexture( const char* texturePath )
{
	ge::GEOPrimitive* render_object = NULL;
	render_object = ge::GEOPrimitive::create();
	if (render_object == NULL) return false;

	ge::GE_VERTEX_DECL* vertex_decl = ge::GEVertexDecl::get_vertex_decl(CC_VERTEX_FVF);
	render_object->set_vertex_decl(vertex_decl);

	bool ret = render_object->create_texture(texturePath);
	if (!ret) return false;

	m_pRenderObject = render_object;
	int width, height;
	render_object->get_texture_size(width, height);
	m_obTextureSize.width = (float)width;
	m_obTextureSize.height = (float)height;
	return ret;
}

void CCTextureAtlas::releaseTexture()
{
	ge::GEOPrimitive* render_object = (ge::GEOPrimitive*)m_pRenderObject;
	if (render_object)
	{
		ge::GEOPrimitive::release(&render_object);
	}

	m_pRenderObject = NULL;
	m_obTextureSize = CCSizeZero;
}

} // namespace CC

namespace ge {

DLL_MANAGE_CLASS_IMPLEMENT(GEOArmature);

GEOArmature::GEOArmature()
: ptr_armature_(NULL)
{

}

GEOArmature::~GEOArmature()
{
	destory();
}

bool GEOArmature::init()
{
	CC::CCDataManager* cc_data_manager = CC::CCDataManager::getInstance();
	if (cc_data_manager == NULL) return false;

	CC::CCDataCollection* cc_data = cc_data_manager->getData("cocos/HeroAnimation.ExportJson");
	ptr_armature_ = new CC::CCArmature();
	ptr_armature_->init(cc_data->getArmatureData());
	ptr_armature_->initAnimation(cc_data->getAnimationData());
	ptr_armature_->initTexture();

	if(CC::CCAnimation* ptr_animation = ptr_armature_->getAnimation())
	{
		ptr_animation->play(0);
		return true;
	} else return false;
}

void GEOArmature::destory()
{
	if (ptr_armature_)
	{
		ptr_armature_->release();
		delete ptr_armature_;
		ptr_armature_ = NULL;
	}
}

void GEOArmature::update( time_t delta )
{
	ptr_armature_->updateAnimation(delta / 1000.f);
	ptr_armature_->updateTransform(delta / 1000.f);
}

void GEOArmature::render( time_t delta )
{
	ptr_armature_->render();
}

} // namespace ge
