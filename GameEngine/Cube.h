#pragma once
#include <array>
#include "Primitive.h"

class Cube : public Primitive
{
private:
	const vec3 a = vec3(-1, 1, -1);
	const vec3 b = vec3(-1, -1, -1);
	const vec3 c = vec3(1, 1, -1);
	const vec3 d = vec3(1, -1, -1);
	const vec3 e = vec3(1, 1, 1);
	const vec3 f = vec3(1, -1, 1);
	const vec3 g = vec3(-1, 1, 1);
	const vec3 h = vec3(-1, -1, 1);

	std::array<vec3, 8> vertex_array = { a,b,c,d,e,f,g,h };
	std::array<unsigned int, 36> ordered_array = {
		0, 2, 1, //Front
		1, 2, 3,
		3, 2, 4, //Right
		3, 4, 5,
		4, 7, 5, //Back
		4, 6, 7,
		7, 6, 0, //Left
		0, 1, 7,
		0, 6, 4, //Up
		0, 4, 2,
		1, 3, 5, //Down
		1, 5, 7
	};

public:
	Cube()
	{
		type = "Cube";

		for (size_t i = 0; i < vertex_array.size(); ++i)
		{
			vertex_data.push_back(Mesh::V3{ vertex_array[i] });
		}
		for (size_t i = 0; i < ordered_array.size(); ++i)
		{
			index_data.push_back(unsigned int{ ordered_array[i] });
		}

		numTexCoords = vertex_data.size();
		numNormals = vertex_data.size();
		numFaces = 12;
	}
};