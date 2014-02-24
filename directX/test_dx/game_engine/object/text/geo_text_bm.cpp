#include "geo_text_bm.h"
#include "../../common/ge_engine.h"
#include "../../render/texture/ge_texture.h"

namespace ge
{
DLL_MANAGE_CLASS_IMPLEMENT(GEOTextBM);

const unsigned GEOTextBM::fvf = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

GEOTextBM::GEOTextBM()
: render_object_(NULL)
, need_update_font_(true)
, need_update_text_(true)
{
}

GEOTextBM::~GEOTextBM()
{
}

bool GEOTextBM::set_font( GEFont* font )
{
	if (font == NULL) return false;
	if (font->type_ != FontType_BMFont) return false;
	bool ret = GEOText::set_font(font);
	need_update_font_ = true;
	return ret;
}

bool GEOTextBM::set_text( const char* text )
{
	bool b_ret = GEOText::set_text(text);
	need_update_text_ = true;
	return b_ret;
}

bool GEOTextBM::update_font()
{
	if (render_object_ == NULL)
	{
		render_object_ = GEOAtlasRender::create();
	} else render_object_->destory();

	GEFontBM* bm_font = (GEFontBM*)font_obj_;
	if (bm_font == NULL) return false;

	int page_cnt = bm_font->get_page_cnt();
	if (render_object_ == NULL) return false;
	for (int i=0; i<page_cnt; ++i)
	{
		char page_path[MAX_PATH];
		bm_font->get_page_path(page_path, i);
		int png_id = render_object_->add_texture(page_path);
		assert(png_id == i);
	}

	need_update_font_ = false;
	return true;
}

bool GEOTextBM::update_text()
{
	GEFontBM* bm_font = (GEFontBM*)font_obj_;
	if (bm_font == NULL) return false;

	if (render_object_ == NULL) return false;

	render_chars_.resize(text_.length());

	bm_font->begin_write(&(render_chars_[0]), render_chars_.max_size());
	bm_font->write_text(text_.c_str(), 400, 0, true);
	int ret = bm_font->end_write();

	if (ret)
	{
		render_object_->clear_quads();
		FOR_EACH (RENDER_CHAR_LIST, render_chars_, char_itor)
		{
			GE_QUAD quad;
			_render_char_to_quad(quad, *char_itor);
			render_object_->add_quad(quad);
		}
	}
	need_update_text_ = false;
	return ret;
}

void GEOTextBM::_clear_render_chars()
{
	render_chars_.clear();
}

void GEOTextBM::_render_char_to_quad( GE_QUAD& out_quad, const bmfont::SCharRenderObject& render_char )
{
	GETexture* texture = render_object_->get_texture(render_char.page);
	if(texture == NULL) return;

	GE_VERTEX* vertex_ptr[4];
	vertex_ptr[0] = &(out_quad.tl);
	vertex_ptr[1] = &(out_quad.tr);
	vertex_ptr[2] = &(out_quad.br);
	vertex_ptr[3] = &(out_quad.bl);

	for (int i=0; i<4; ++i)
	{
		vertex_ptr[i]->set_decl(render_object_->get_vertex_decl());
		vertex_ptr[i]->set_color(0xffffffff);
	}

	float min_x = render_char.xys[0];
	float min_y = render_char.xys[1];
	float max_x = render_char.xys[2];
	float max_y = render_char.xys[3];

	out_quad.tl.set_position(D3DXVECTOR3(min_x, min_y, 0.f));
	out_quad.tr.set_position(D3DXVECTOR3(max_x, min_y, 0.f));
	out_quad.br.set_position(D3DXVECTOR3(max_x, max_y, 0.f));
	out_quad.bl.set_position(D3DXVECTOR3(min_x, max_y, 0.f));

	float u1 = render_char.uvs[0];
	float v1 = render_char.uvs[1];
	float u2 = render_char.uvs[2];
	float v2 = render_char.uvs[3];

	out_quad.tl.set_texcoords(D3DXVECTOR2(u1, v1));
	out_quad.tr.set_texcoords(D3DXVECTOR2(u2, v1));
	out_quad.br.set_texcoords(D3DXVECTOR2(u2, v2));
	out_quad.bl.set_texcoords(D3DXVECTOR2(u1, v2));

	out_quad.texture = render_char.page;

	return;
}

void GEOTextBM::render( time_t delta )
{
	if (need_update_font_) update_font();
	if (need_update_text_) update_text();

	render_object_->render(delta);
}

}

