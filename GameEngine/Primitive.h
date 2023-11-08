#pragma once

#include <vector>
#include <glm/vec4.hpp>

#include "Mesh.h"

class Primitive
{
public:
	enum Mesh::Formats _format = Mesh::Formats::F_V3;
	glm::vec4 Color = glm::vec4(255, 255, 255, 255);

protected:
	std::vector<Mesh::V3> vertex_data;
	std::vector<unsigned int> index_data;

	unsigned int numTexCoords; // Number of texture coordinates is same as the number of vertices
	unsigned int numNormals; // Number of normals is same as the number of vertices
	unsigned int numFaces;

	std::string type;


public:
	std::vector<Mesh::V3>* getVertexData() { return &vertex_data; }
	std::vector<unsigned int>* getIndexData() { return &index_data; }

	unsigned int GetNumTexCoords() { return numTexCoords; }
	unsigned int GetNumNormals() { return numNormals; }
	unsigned int GetNumFaces() { return numFaces; }

	std::string GetType() { return type; }
};