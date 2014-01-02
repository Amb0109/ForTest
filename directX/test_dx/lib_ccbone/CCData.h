#ifndef _CC_DATA_LOADER_H_
#define _CC_DATA_LOADER_H_

#include <string>
#include <vector>
#include "./support/json/json_lib.h"

#define VERSION_CHANGE_ROTATION_RANGE 1.0f
#define VERSION_COLOR_READING 1.1f

#ifndef FOR_EACH
#define FOR_EACH(type, obj, it) for (type::iterator it = obj.begin(); it != obj.end(); ++it)
#endif

static const char *VERSION = "version";
static const float VERSION_2_0 = 2.0f;

static const char *ARMATURES = "armatures";
static const char *ARMATURE = "armature";
static const char *BONE = "b";
static const char *DISPLAY = "d";

static const char *ANIMATIONS = "animations";
static const char *ANIMATION = "animation";
static const char *MOVEMENT = "mov";
static const char *FRAME = "f";

static const char *TEXTURE_ATLAS = "TextureAtlas";
static const char *SUB_TEXTURE = "SubTexture";

static const char *A_NAME = "name";
static const char *A_DURATION = "dr";
static const char *A_FRAME_INDEX = "fi";
static const char *A_DURATION_TO = "to";
static const char *A_DURATION_TWEEN = "drTW";
static const char *A_LOOP = "lp";
static const char *A_MOVEMENT_SCALE = "sc";
static const char *A_MOVEMENT_DELAY = "dl";
static const char *A_DISPLAY_INDEX = "dI";

// static const char *A_VERT = "vert";
// static const char *A_FRAG = "frag";
static const char *A_PLIST = "plist";

static const char *A_PARENT = "parent";
static const char *A_SKEW_X = "kX";
static const char *A_SKEW_Y = "kY";
static const char *A_SCALE_X = "cX";
static const char *A_SCALE_Y = "cY";
static const char *A_Z = "z";
static const char *A_EVENT = "evt";
static const char *A_SOUND = "sd";
static const char *A_SOUND_EFFECT = "sdE";
static const char *A_TWEEN_EASING = "twE";
//static const char *A_TWEEN_ROTATE = "twR";
static const char *A_IS_ARMATURE = "isArmature";
static const char *A_DISPLAY_TYPE = "displayType";
static const char *A_MOVEMENT = "mov";

static const char *A_X = "x";
static const char *A_Y = "y";

static const char *A_COCOS2DX_X = "cocos2d_x";
static const char *A_COCOS2DX_Y = "cocos2d_y";

static const char *A_WIDTH = "width";
static const char *A_HEIGHT = "height";
static const char *A_PIVOT_X = "pX";
static const char *A_PIVOT_Y = "pY";

static const char *A_COCOS2D_PIVOT_X = "cocos2d_pX";
static const char *A_COCOS2D_PIVOT_Y = "cocos2d_pY";

static const char *A_BLEND_TYPE = "bd";

static const char *A_ALPHA = "a";
static const char *A_RED = "r";
static const char *A_GREEN = "g";
static const char *A_BLUE = "b";
static const char *A_ALPHA_OFFSET = "aM";
static const char *A_RED_OFFSET = "rM";
static const char *A_GREEN_OFFSET = "gM";
static const char *A_BLUE_OFFSET = "bM";
static const char *A_COLOR_TRANSFORM = "colorTransform";
static const char *A_TWEEN_FRAME = "tweenFrame";

static const char *CONTOUR = "con";
static const char *CONTOUR_VERTEX = "con_vt";

static const char *FL_NAN = "NaN";

static const char *FRAME_DATA = "frame_data";
static const char *MOVEMENT_BONE_DATA = "mov_bone_data";
static const char *MOVEMENT_DATA = "mov_data";
static const char *ANIMATION_DATA = "animation_data";
static const char *DISPLAY_DATA = "display_data";
static const char *SKIN_DATA = "skin_data";
static const char *BONE_DATA = "bone_data";
static const char *ARMATURE_DATA = "armature_data";
static const char *CONTOUR_DATA = "contour_data";
static const char *TEXTURE_DATA = "texture_data";
static const char *VERTEX_POINT = "vertex";
static const char *COLOR_INFO = "color";

static const char *CONFIG_FILE_PATH = "config_file_path";
static const char *CONTENT_SCALE = "content_scale";

class CCArmatureData;
class CCAnimationData;
class CCTextureData;
class CCAtlasData;
class CCData;

typedef std::map<std::string, int> DATA_NAME_MAP;

class CCBaseData
{
public:
	bool		load(Json::Value& json, CCData* data);
	void		copy(const CCBaseData* node_data);

public:
	float		x;
	float		y;
	int			zOrder;
	float		scaleX;
	float		scaleY;
	float		skewX;
	float		skewY;

	float		tweenRotate;

	bool		isUseColorInfo;
	unsigned	colorInfo;
};


class CCData
{
public:
	CCData();
	virtual ~CCData();

public:
	bool				load_file(const char* file_path);
	bool				load(const char* file_data);
	void				clear();

	void				set_position_scale(float scale) {m_position_scale = scale;}

	std::string&		get_name() { return m_name; }
	CCArmatureData*		get_armature_data(int index = 0);
	CCArmatureData*		get_armature_data_by_name(const char* name);
	CCAnimationData*	get_animation_data(int index = 0);
	CCAnimationData*	get_animation_data_by_name(const char* name);
	CCTextureData*		get_texture_data(int index = 0);
	CCTextureData*		get_texture_data_by_name(const char* name);
	CCAtlasData*		get_atlas_data(int index = 0);
	int					get_atlas_cnt() {return (int)m_atlas_data.size();}

public:
	std::vector<CCArmatureData*>	m_armature_data;
	std::vector<CCAnimationData*>	m_animation_data;
	std::vector<CCTextureData*>		m_texture_data;
	std::vector<CCAtlasData*>		m_atlas_data;
	DATA_NAME_MAP					m_armature_data_map;
	DATA_NAME_MAP					m_animation_data_map;
	DATA_NAME_MAP					m_texture_data_map;

	std::string						m_name;
	float							m_version;
	float							m_content_scale;
	float							m_position_scale;
	std::string						m_base_path;
	std::string						m_path;
};



#endif // _CC_DATA_LOADER_H_