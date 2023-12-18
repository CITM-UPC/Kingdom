#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include "Engine_Globals.h"

using namespace std;
class MeshInfo
{
public:

	struct V3 { vec3f v; };
	struct V3C4 { vec3f v; vec4f c; };
	struct V3T2 { vec3f v; vec2f t; };

	MeshInfo(const void* vertex_data, unsigned int numVerts,
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

	const unsigned int _numVerts, _numIndexs, _numTexCoords,
		_numNormals, _numFaces;

	std::vector<vec3f> mVertices;
	std::vector<vec3f> mNormals;
	std::vector<vec3f> mFaceCenters;
	std::vector<vec3f> mFaceNormals;

	ostream& serialize(ostream& os) const {
	
		os.write((char*)&_numVerts, sizeof(_numVerts));
		os.write((char*)_vertex_data, _numVerts * sizeof(V3T2)); // we need to control this somehow

		os.write((char*)&_numIndexs, sizeof(_numIndexs));
		os.write((char*)_index_data, _numIndexs * sizeof(uint));

		os.write((char*)&_numTexCoords, sizeof(_numTexCoords));

		os.write((char*)mVertices.data(), _numVerts * sizeof(vec3f));

		os.write((char*)&_numNormals, sizeof(_numNormals));
		os.write((char*)mNormals.data(), _numNormals * sizeof(vec3f));

		os.write((char*)&_numFaces, sizeof(_numFaces));
		os.write((char*)mFaceNormals.data(), _numFaces * sizeof(vec3f));
		os.write((char*)mFaceCenters.data(), _numFaces * sizeof(vec3f));

		return os;
	}

	istream& deserialize(istream& is) {
		is.read((char*)&_numVerts, sizeof(_numVerts));

		// Allocate memory for _vertex_data and read the data
		_vertex_data = new V3T2[_numVerts];
		is.read((char*)_vertex_data, _numVerts * sizeof(V3T2));

		is.read((char*)&_numIndexs, sizeof(_numIndexs));

		// Allocate memory for _index_data and read the data
		_index_data = new uint[_numIndexs];
		is.read((char*)_index_data, _numIndexs * sizeof(uint));

		is.read((char*)&_numTexCoords, sizeof(_numTexCoords));

		// Read mVertices
		mVertices.resize(_numVerts);
		is.read((char*)mVertices.data(), _numVerts * sizeof(vec3f));

		is.read((char*)&_numNormals, sizeof(_numNormals));

		// Read mNormals
		mNormals.resize(_numNormals);
		is.read((char*)mNormals.data(), _numNormals * sizeof(vec3f));

		is.read((char*)&_numFaces, sizeof(_numFaces));

		// Read mFaceNormals and mFaceCenters
		mFaceNormals.resize(_numFaces);
		mFaceCenters.resize(_numFaces);
		is.read((char*)mFaceNormals.data(), _numFaces * sizeof(vec3f));
		is.read((char*)mFaceCenters.data(), _numFaces * sizeof(vec3f));

		return is;
	}

};

inline ostream& operator<<(ostream& os, const MeshInfo& mesh) {
	return mesh.serialize(os);

}

inline istream& operator>>(istream& is, MeshInfo& mesh) {
	return mesh.deserialize(is);
}