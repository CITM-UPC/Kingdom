#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include "Engine_Globals.h"

using namespace std;
class MeshInfo
{
public:

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
		//size_t vLength = vertex_data.size();
		//os.write((char*)&vLength, sizeof(vLength));
		//os.write((char*)vertex_data.data(), vertex_data.size() * sizeof(VertexV3T2));

		//size_t iLength = index_data.size();
		//os.write((char*)&iLength, sizeof(iLength));
		//os.write((char*)index_data.data(), index_data.size() * sizeof(uint));

		return os;
	}

	istream& deserialize(istream& is) {
		//size_t temp = 0;
		//is.read((char*)&temp, sizeof(temp));
		//vertex_data.resize(temp); // allocates temp amount of cells and changes the length of the vector
		//is.read((char*)vertex_data.data(), temp * sizeof(VertexV3T2));

		//is.read((char*)&temp, sizeof(temp));
		//index_data.resize(temp);
		//is.read((char*)index_data.data(), temp * sizeof(uint));

		return is;
	}

};

inline ostream& operator<<(ostream& os, const MeshInfo& mesh) {
	return mesh.serialize(os);

}

inline istream& operator>>(istream& is, MeshInfo& mesh) {
	return mesh.deserialize(is);
}