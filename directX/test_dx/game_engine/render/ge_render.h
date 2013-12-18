#ifndef _GAME_ENGINE_RENDER_H_
#define _GAME_ENGINE_RENDER_H_

#include "../common/ge_include.h"
#include "../utility/geu_gmath.h"
#include "ger_font.h"

namespace ge
{

class GE_API GEObject;
class GE_API GERender
{
protected:
	typedef std::queue<GEObject*> RENDER_TASK_QUE;

public:
	GERender();
	virtual ~GERender();

public:
	bool init();
	void render(time_t time_elapsed);
	void release();

	void push_render(GEObject* p_object);

	virtual bool do_view_trans(D3DXVECTOR3& position, D3DXVECTOR3& target, D3DXVECTOR3& up);
	virtual bool do_projection_trans(float fovy);
	
	D3DXMATRIX&	get_word_view_matrix() {return word_view_matrix_;}
	D3DXMATRIX& get_word_view_proj_matrix() {return word_view_proj_matrix_;}

	virtual bool set_render_state(D3DRENDERSTATETYPE type, DWORD value);

protected:
	RENDER_TASK_QUE		render_task_que_;

	D3DXMATRIX			word_view_matrix_;
	D3DXMATRIX			word_view_proj_matrix_;
};

} // namespace ge

#endif // _GAME_ENGINE_RENDER_H_