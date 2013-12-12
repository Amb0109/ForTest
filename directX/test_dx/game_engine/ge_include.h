#ifndef _GAME_ENGINE_INCLUDE_H_
#define _GAME_ENGINE_INCLUDE_H_

#include <cstdio>
#include <cstring>
#include <cstdlib>

// win api include
#include <Windows.h>

// d3d include
#include <d3d9.h>
#include <d3dx9.h>


#define GE_API __declspec(dllexport)


#define DEF_WND_STYLE ((WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_CLIPCHILDREN | WS_VISIBLE)

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } } 

#endif // _GAME_ENGINE_INCLUDE_H_