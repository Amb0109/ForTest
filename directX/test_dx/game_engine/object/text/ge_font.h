#ifndef _GAME_ENGINE_OBJECT_FONT_H_
#define _GAME_ENGINE_OBJECT_FONT_H_

#include "../../common/ge_include.h"
#include "../../utility/geu_gmath.h"

namespace ge
{

enum GE_API GEFontType
{
	FontType_Default = 0,
	FontType_D3DXFont,
	FontType_GDIFont,
	FontType_BMFont,
	FontType_FTFont,
};

class GE_API GEFont
{
public:
	GEFont();
	virtual ~GEFont();

public:
	virtual bool init(const char* face, int size);
	virtual void destory();

protected:
	virtual bool _init_font();

public:
	GEFontType	type_;

	char		face_[32];
	int			size_;
	int			charset_;
	int			weight_;
	bool		italic_;
	bool		underline_;
	bool		strikeout_;
};

class GE_API GED3DXFont : public GEFont
{
public:
	GED3DXFont();
	virtual ~GED3DXFont();

public:
	LPD3DXFONT get_d3dx_obj();

	virtual void destory();

protected:
	virtual bool _init_font();

private:
	D3DXFONT_DESC	d3dx_font_desc_;
	LPD3DXFONT		d3dx_font_;
};

class GE_API GEGDIFont : public GEFont
{
public:
	GEGDIFont();
	virtual ~GEGDIFont();

public:
	HFONT get_gdi_obj();

	virtual void destory();

protected:
	virtual bool _init_font();

private:
	LOGFONT			gdi_font_desc_;
	HFONT			gdi_font_;
};

class GE_API GEFontManager
{
protected:
	typedef std::set<GEFont*>	GE_FONT_SET;

	GEFontManager();
	virtual ~GEFontManager();

public:
	static GEFontManager* get_instance();
	static GEFont* create_font(GEFontType font_type);
	static void release_font(GEFont* ptr_font);

public:
	GEFont* new_font(GEFontType font_type);
	void	delete_font(GEFont* ptr_font);

protected:
	GE_FONT_SET				font_set_;
};


} // namespace ge

#endif // _GAME_ENGINE_OBJECT_FONT_H_