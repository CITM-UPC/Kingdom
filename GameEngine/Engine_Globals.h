#pragma once
#include <windows.h>
#include <stdio.h>

#define ENGINE_LOG(format, ...) engineLog(__FILE__, __LINE__, format, __VA_ARGS__);

void engineLog(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI


typedef unsigned int uint;

enum engine_status
{
	ENGINE_UPDATE_CONTINUE = 1,
	ENGINE_UPDATE_STOP,
	ENGINE_UPDATE_ERROR
};
