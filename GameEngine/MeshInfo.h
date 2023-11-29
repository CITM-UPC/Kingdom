#pragma once
#include <vector>
#include "Engine_Globals.h"

class MeshInfo
{
public:

	inline MeshInfo(const void* vertex_data, unsigned int numVerts, 
					const unsigned int* index_data = nullptr, unsigned int numIndexs = 0,
					const unsigned int numTexCoords = 0, unsigned int numNormals = 0, unsigned int numFaces = 0) :
		_vertex_data(vertex_data),
		_numVerts(numVerts),
		_index_data(index_data),
		_numIndexs(numIndexs),
		_numTexCoords(numTexCoords),
		_numNormals(numNormals),
		_numFaces(numFaces) 
	{
	}

	const void* _vertex_data;
	const unsigned int* _index_data;

	const unsigned int _numVerts,	_numIndexs,	_numTexCoords, 
					   _numNormals,	_numFaces;

	std::vector<vec3f> mVertices;
	std::vector<vec3f> mNormals;
	std::vector<vec3f> mFaceCenters;
	std::vector<vec3f> mFaceNormals;
};