#include "geo_text_bm.h"
#include "../../common/ge_engine.h"
#include "../texture/ge_texture.h"

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

	GEBMFont* bm_font = (GEBMFont*)font_obj_;
	if (bm_font == NULL) return false;

	int png_cnt = bm_font->get_png_cnt();
	if (render_object_ == NULL) return false;
	for (int i=0; i<png_cnt; ++i)
	{
		char png_path[MAX_PATH];
		bm_font->get_png_path(png_path, i);
		int png_id = render_object_->add_texture(png_path);
		assert(png_id == i);
	}

	need_update_font_ = false;
	return true;
}

bool GEOTextBM::update_text()
{
	GEBMFont* bm_font = (GEBMFont*)font_obj_;
	if (bm_font == NULL) return false;

	if (render_object_ == NULL) return false;

	bool ret = bm_font->compose(this, text_.c_str(), 0, 0, false);
	if (ret)
	{
		render_object_->clear_quads();
		FOR_EACH (RENDER_CHAR_LIST, render_chars_, char_itor)
		{
			GE_QUAD quad;
			_text_char_to_quad(quad, *char_itor);
			render_object_->add_quad(quad);
		}
	}
	need_update_text_ = false;
	return ret;
}

void GEOTextBM::_add_render_char( GE_TEXT_CHAR& text_char )
{
	render_chars_.push_back(text_char);
}

void GEOTextBM::_clear_render_chars()
{
	render_chars_.clear();
}

void GEOTextBM::_text_char_to_quad( GE_QUAD& out_quad, const GE_TEXT_CHAR& text_char )
{
	GETexture* texture = render_object_->get_texture(text_char.img_);
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

	float min_x = (float)text_char.pos_.x;
	float min_y = (float)text_char.pos_.y;
	float max_x = min_x + text_char.size_.width;
	float max_y = min_y + text_char.size_.height;

	out_quad.tl.set_position(D3DXVECTOR3(min_x, -min_y, 0.f));
	out_quad.tr.set_position(D3DXVECTOR3(max_x, -min_y, 0.f));
	out_quad.br.set_position(D3DXVECTOR3(max_x, -max_y, 0.f));
	out_quad.bl.set_position(D3DXVECTOR3(min_x, -max_y, 0.f));

	int img_width = 0;
	int img_height = 0;
	texture->get_texture_size(img_width, img_height);

	float u1 = (float)text_char.img_pos_.x / img_width;
	float v1 = (float)text_char.img_pos_.y / img_height;
	float u2 = ((float)text_char.img_pos_.x + text_char.size_.width) / img_width;
	float v2 = ((float)text_char.img_pos_.y + text_char.size_.height + 1) / img_height;

	out_quad.tl.set_texcoords(D3DXVECTOR2(u1, v1));
	out_quad.tr.set_texcoords(D3DXVECTOR2(u2, v1));
	out_quad.br.set_texcoords(D3DXVECTOR2(u2, v2));
	out_quad.bl.set_texcoords(D3DXVECTOR2(u1, v2));

	out_quad.texture = text_char.img_;

	return;
}

void GEOTextBM::render( time_t delta )
{
	if (need_update_font_) update_font();
	if (need_update_text_) update_text();

	render_object_->render(delta);
}

}

