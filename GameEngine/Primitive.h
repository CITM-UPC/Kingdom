#pragma once

#include <vector>
#include <glm/vec4.hpp>

#include "Mesh.h"

class Primitive
{
public:

	enum class Type
	{
		CUBE,
		CYLINDER,
		SPHERE,
		PYRAMID
	};

	static const int NUM_FACES = 6;
	static const int NUM_TRIANGLES = NUM_FACES * 2;
	static const int NUM_VERTEXS = NUM_TRIANGLES * 3;

	const enum Formats _format;
	std::vector<Mesh::V3> vertex_data;

	glm::vec4 Color;
};