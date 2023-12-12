#include "Mesh.h"
#include <GL/glew.h>
#include <iostream>

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <vector>
#include <array>

#include "GameObject.h"

using namespace std;

Mesh::Mesh(GameObject* owner, Formats format, const void* vertex_data, unsigned int numVerts, const unsigned int* index_data, unsigned int numIndexs, const unsigned int numTexCoords, unsigned int numNormals, unsigned int numFaces) :
	Component(owner),
	_format(format),
	_numVerts(numVerts),
	_numIndexs(numIndexs),
	_numNormals(numNormals),
	_numTexCoords(numTexCoords),
	_numFaces(numFaces)
{
	glGenBuffers(1, &_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);

	switch (_format) {
	case Formats::F_V3:
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3) * numVerts, vertex_data, GL_STATIC_DRAW);
		break;
	case Formats::F_V3C4:
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3C4) * numVerts, vertex_data, GL_STATIC_DRAW);
		break;
	case Formats::F_V3T2:
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3T2) * numVerts, vertex_data, GL_STATIC_DRAW);
		break;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (index_data) {
		glGenBuffers(1, &_indexs_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexs_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndexs, index_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		_indexs_buffer_id = 0;
	}
}

Mesh::Mesh(GameObject* owner, const MeshInfo& meshinfo, Formats format) :
	Component(owner),
	_format(format),
	_numVerts(meshinfo._numVerts),
	_numIndexs(meshinfo._numIndexs),
	_numNormals(meshinfo._numNormals),
	_numTexCoords(meshinfo._numTexCoords),
	_numFaces(meshinfo._numFaces),
	mVertices(meshinfo.mVertices),
	mNormals(meshinfo.mNormals),
	mFaceCenters(meshinfo.mFaceCenters),
	mFaceNormals(meshinfo.mFaceNormals)
{
	glGenBuffers(1, &_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);

	switch (_format) {
	case Formats::F_V3:
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3) * meshinfo._numVerts, meshinfo._vertex_data, GL_STATIC_DRAW);
		break;
	case Formats::F_V3C4:
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3C4) * meshinfo._numVerts, meshinfo._vertex_data, GL_STATIC_DRAW);
		break;
	case Formats::F_V3T2:
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3T2) * meshinfo._numVerts, meshinfo._vertex_data, GL_STATIC_DRAW);
		break;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (meshinfo._index_data) {
		glGenBuffers(1, &_indexs_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexs_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshinfo._numIndexs, meshinfo._index_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		_indexs_buffer_id = 0;
	}
}

Mesh::Mesh(Mesh&& b) noexcept :
	Component(b.owner),
	_format(b._format),
	_vertex_buffer_id(b._vertex_buffer_id),
	_numVerts(b._numVerts),
	_indexs_buffer_id(b._indexs_buffer_id),
	_numIndexs(b._numIndexs),
	_numTexCoords(b._numTexCoords),
	_numNormals(b._numNormals),
	_numFaces(b._numFaces),
	texture(b.texture),
	mVertices(b.mVertices),
	mNormals(b.mNormals),
	mFaceCenters(b.mFaceCenters),
	mFaceNormals(b.mFaceNormals)
{
	b._vertex_buffer_id = 0;
	b._indexs_buffer_id = 0;
}

Mesh::Mesh(const Mesh& cpy) :
	Component(cpy.owner),
	meshName(cpy.meshName),
	_format(cpy._format),
	_vertex_buffer_id(cpy._vertex_buffer_id),
	_numVerts(cpy._numVerts),
	_indexs_buffer_id(cpy._indexs_buffer_id),
	_numIndexs(cpy._numIndexs),
	_numTexCoords(cpy._numTexCoords),
	_numNormals(cpy._numNormals),
	_numFaces(cpy._numFaces),
	texture(cpy.texture),    // Copies the shared_ptr, so it now points to the same object
	mVertices(cpy.mVertices),
	mNormals(cpy.mNormals),
	mFaceCenters(cpy.mFaceCenters),
	mFaceNormals(cpy.mFaceNormals)
{
	const_cast<Mesh&>(cpy)._vertex_buffer_id = 0;
	const_cast<Mesh&>(cpy)._indexs_buffer_id = 0;
}

void Mesh::draw() {

	glPushMatrix();
	glMultMatrixd(&owner->GetComponent<Transform>()->_transformationMatrix[0].x);

	glColor4ub(255, 255, 255, 255);

	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);
	glEnableClientState(GL_VERTEX_ARRAY);

	switch (_format) {
	case Formats::F_V3:
		glVertexPointer(3, GL_FLOAT, 0, nullptr);
		break;
	case Formats::F_V3C4:
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(V3C4), nullptr);
		glColorPointer(4, GL_FLOAT, sizeof(V3C4), (void*)sizeof(V3));
		break;
	case Formats::F_V3T2:
		glEnable(GL_TEXTURE_2D);
		(texture) ? (useChecker) ? texture->checker()
			: texture->bind()
			: texture->unbind();

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(V3T2), nullptr);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3T2), (void*)sizeof(V3));
		break;
	}

	if (_indexs_buffer_id) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexs_buffer_id);
		glDrawElements(GL_TRIANGLES, _numIndexs, GL_UNSIGNED_INT, nullptr);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, _numVerts);
	}

	if (drawVertexNormals) {
		glLineWidth(4.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);

		for (int i = 0; i < _numVerts; i++) {
			glVertex3f(mVertices[i].x, mVertices[i].y, mVertices[i].z);
			glVertex3f(mVertices[i].x + mNormals[i].x * 0.1f,
				mVertices[i].y + mNormals[i].y * 0.1f,
				mVertices[i].z + mNormals[i].z * 0.1f);
		}

		glColor3f(0.0f, 1.0f, 0.0f);
		glEnd();
	}

	if (drawFaceNormals) {
		glLineWidth(4.0f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);

		for (int i = 0; i < _numFaces; i++) {
			glm::vec3 endPoint = mFaceCenters[i] + 0.1f * mFaceNormals[i];
			glVertex3f(mFaceCenters[i].x, mFaceCenters[i].y, mFaceCenters[i].z);
			glVertex3f(endPoint.x, endPoint.y, endPoint.z);
		}

		glColor3f(1.0f, 0.0f, 0.0f);
		glEnd();
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

Mesh::~Mesh() {
	if (_vertex_buffer_id)
		glDeleteBuffers(1, &_vertex_buffer_id);
	if (_indexs_buffer_id)
		glDeleteBuffers(1, &_indexs_buffer_id);
}

void Mesh::setName(std::string name) {
	meshName = name;
}

std::string Mesh::getName() {
	return meshName;
}

const unsigned int Mesh::getNumVerts() {
	return _numVerts;
}

const unsigned int Mesh::getNumIndexs() {
	return _numIndexs;
}

const unsigned int Mesh::getNumTexCoords() {
	return _numTexCoords;
}

const unsigned int Mesh::getNumNormals() {
	return _numNormals;
}

const unsigned int Mesh::getNumFaces() {
	return _numFaces;
}

void Mesh::Update()
{
	draw();
}