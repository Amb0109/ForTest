#ifndef __CC_MACRO_H__
#define __CC_MACRO_H__

#ifndef MEMBER_GET_PTR
#define MEMBER_GET_PTR(_type, _name, _func) \
public:	\
	_type get##_func() const { return _name; } \
protected: \
	_type	_name;
#endif

#ifndef MEMBER_GET_CREF
#define MEMBER_GET_CREF(_type, _name, _func) \
public:	\
	const _type& get##_func() const { return _name; } \
protected: \
	_type	_name;
#endif

#ifndef MEMBER_GET_SET_PTR
#define MEMBER_GET_SET_PTR(_type, _name, _func) \
public:	\
	void set##_func(_type val) { _name = val; } \
	_type get##_func() { return _name; } \
protected: \
	_type	_name;
#endif

#ifndef MEMBER_GET_SET_CREF
#define MEMBER_GET_SET_CREF(_type, _name, _func) \
public:	\
	void set##_func(_type val) { _name = val; } \
	const _type& get##_func() const { return _name; } \
protected: \
	_type	_name;
#endif

#ifndef FOR_EACH
#define FOR_EACH(type, obj, it) for (type::iterator it = obj.begin(); it != obj.end(); ++it)
#endif

#ifdef DLL_EXPORTS
#define CC_DLL __declspec(dllexport)
#else
#define CC_DLL
#endif
#pragma warning (disable:4251) // 忽略dll导出模板类的warning

// cmath define
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef M_PI_X_2
#define M_PI_X_2 (M_PI * 2.f)
#endif

// cocostdio version control
#define VERSION_CHANGE_ROTATION_RANGE 1.0f
#define VERSION_COLOR_READING 1.1f

#endif //__CC_MACRO_H__