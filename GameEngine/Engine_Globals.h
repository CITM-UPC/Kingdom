#pragma once
#include <windows.h>
#include <stdio.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define ENGINE_LOG(format, ...) engineLog(__FILE__, __LINE__, format, __VA_ARGS__);

void engineLog(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;

typedef glm::dvec2 vec2;
typedef glm::dvec3 vec3;
typedef glm::dvec4 vec4;
typedef glm::dmat4 mat4;

typedef glm::vec2 vec2f;
typedef glm::vec3 vec3f;
typedef glm::vec4 vec4f;

enum engine_status
{
	ENGINE_UPDATE_CONTINUE = 1,
	ENGINE_UPDATE_STOP,
	ENGINE_UPDATE_ERROR
};
