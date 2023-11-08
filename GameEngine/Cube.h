#pragma once
#include "Primitive.h"

class Cube : public Primitive
{
private:
	const vec3 a = vec3(-1, -1, 1);
	const vec3 b = vec3(1, -1, 1);
	const vec3 c = vec3(1, 1, 1);
	const vec3 d = vec3(-1, 1, 1);
	const vec3 e = vec3(-1, -1, -1);
	const vec3 f = vec3(1, -1, -1);
	const vec3 g = vec3(1, 1, -1);
	const vec3 h = vec3(-1, 1, -1);

public:
	Cube()
	{
		vertex_data.push_back(Mesh::V3{ a });
	}
};