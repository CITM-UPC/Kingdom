#pragma once
#include <vector>
#include "Engine_Globals.h"

class MeshInfo
{
public:

	MeshInfo(unsigned int numVerts, unsigned int numIndexs = 0,
			 const unsigned int numTexCoords = 0, unsigned int numNormals = 0, unsigned int numFaces = 0) :
		_numVerts(numVerts),
		_numIndexs(numIndexs),
		_numTexCoords(numTexCoords),
		_numNormals(numNormals),
		_numFaces(numFaces) 
	{
	}

	const unsigned int _numVerts,	_numIndexs,	_numTexCoords, 
					   _numNormals,	_numFaces;

	std::vector<vec3f> mVertices;
	std::vector<vec3f> mNormals;
	std::vector<vec3f> mFaceCenters;
	std::vector<vec3f> mFaceNormals;
};