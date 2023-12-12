#pragma once
#include <vector>
#include <glm/trigonometric.hpp>
#include "Primitive.h"

class Cone : public Primitive
{
private:
	const vec3 topCenter = vec3(0, 1.5, 0);
	const vec3 bottomCenter = vec3(0, 0, 0);

	std::vector<vec3> vertex_array;


public:
	Cone(int faces)
	{
		type = "Cone";

		//Adding vertices
		vertex_array.push_back(topCenter);
		vertex_array.push_back(bottomCenter);

		double step = 360.0 / faces;
		double stepProcess = 0;

		for (unsigned int i = 0; i < faces; ++i)
		{
			vertex_array.push_back(vec3(glm::cos(glm::radians(stepProcess)), 0, glm::sin(glm::radians(stepProcess))));
			stepProcess += step;
		}
		for (unsigned int i = 0; i < vertex_array.size(); ++i)
		{
			vertex_data.push_back(Mesh::V3{ vertex_array[i] });
		}


		//Seting the indexes
		for (unsigned int i = 3; i < faces + 2; ++i)
		{
			//Cone faces
			index_data.push_back(unsigned int{ 0 });
			index_data.push_back(unsigned int{ i });
			index_data.push_back(unsigned int{ i - 1 });
			//Base faces
			index_data.push_back(unsigned int{ i });
			index_data.push_back(unsigned int{ 1 });
			index_data.push_back(unsigned int{ i - 1 });
		}
		//Last cone face (goes back to the first circular vertex)
		index_data.push_back(unsigned int{ (unsigned int)0 });
		index_data.push_back(unsigned int{ (unsigned int)2 });
		index_data.push_back(unsigned int{ (unsigned int)faces + 1 });

		//Last base face (goes back to the first circular vertex)
		index_data.push_back(unsigned int{ (unsigned int)2 });
		index_data.push_back(unsigned int{ (unsigned int)1 });
		index_data.push_back(unsigned int{ (unsigned int)faces + 1 });


		numTexCoords = vertex_data.size();
		numNormals = vertex_data.size();
		numFaces = 12;
	}
};