#pragma once
#include <array>
#include "Primitive.h"

class Pyramid : public Primitive
{
private:
	const vec3 a = vec3(0, 1.5, 0);
	const vec3 b = vec3(-1, 0, -1);
	const vec3 c = vec3(1, 0, -1);
	const vec3 d = vec3(1, 0, 1);
	const vec3 e = vec3(-1, 0, 1);

	std::array<vec3, 5> vertex_array = { a,b,c,d,e };
	std::array<unsigned int, 18> ordered_array = {
		0, 2, 1, //Front
		0, 3, 2, //Right
		0, 4, 3, //Back
		0, 1, 4, //Left
		1, 2, 4, //Bottom
		2, 3, 4,
	};

public:
	Pyramid()
	{
		type = "Pyramid";

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