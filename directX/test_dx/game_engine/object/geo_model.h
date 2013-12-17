#ifndef _GAME_ENGINE_OBJECT_MODEL_H_
#define _GAME_ENGINE_OBJECT_MODEL_H_

#include "../common/ge_include.h"
#include "../object/ge_object.h"

namespace ge
{

class GE_API GEOModel : public GEObject
{
public:
	GEOModel();
	virtual ~GEOModel();

public:
	virtual bool create_vetrix_buff(int buff_size);
	virtual bool set_vetrix(int id, GE_VERTEX& vertex);
	virtual bool set_vertices(GE_VERTEX* vertex_array, int size);
	virtual void release_vetrix_buff();

	virtual bool create_index_buff(int buff_size);
	virtual bool set_index(int id, WORD index);
	virtual bool set_indices(WORD* vertex_array, int size);
	virtual void release_index_buff();

public:
	virtual bool init();
	virtual void destory();

	virtual void update(time_t time_elapsed);
	virtual void render(time_t time_elapsed);

protected:
	IDirect3DVertexBuffer9*	d3d_vertex_buff_;
	IDirect3DIndexBuffer9*	d3d_index_buff_;
	int						vertex_buff_size_;
	int						index_buff_size_;
};

} // namespace ge

#endif // _GAME_ENGINE_OBJECT_MODEL_H_