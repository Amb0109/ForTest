#ifndef _GAME_ENGINE_INCLUDE_H_
#define _GAME_ENGINE_INCLUDE_H_

#ifdef DLLIMPORT
#define GE_API __declspec(dllimport)
#else
#define GE_API __declspec(dllexport)
#endif

#pragma warning (disable:4251) // 忽略dll导出模板类的warning

// c std include
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <assert.h>

// stl include
#include <vector>
#include <queue>
#include <map>

// win api include
#include <Windows.h>

// d3d include
#include <d3d9.h>
#include <d3dx9.h>

#define DEF_WND_STYLE ((WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_CLIPCHILDREN | WS_VISIBLE)

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } } 

#define RGBA(r, g, b, a)   ((D3DCOLOR) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

#define FOR_EACH(type, obj, it) for (type::iterator it = obj.begin(); it != obj.end(); ++it)


#endif // _GAME_ENGINE_INCLUDE_H_