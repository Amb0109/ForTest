#ifndef _TARRAY_H_
#define _TARRAY_H_

#include "../lua_engine.h"

typedef struct Point Point;
struct Point
{
	float x;
	float y;
};


extern int a[10];
extern Point p[10]; 
extern Point* pp[10];


extern int ma[10];
extern Point mp[10]; 
extern Point* mpp[10];

typedef struct Array Array;
struct Array
{
 int a[10];
 Point p[10]; 
 Point* pp[10];
};

extern Array array;
extern Array* parray;

void run_tarray(LuaEngine* LE);

#endif