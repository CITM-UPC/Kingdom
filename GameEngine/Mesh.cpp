#include "Mesh.h"
#include <GL/glew.h>

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <vector>
#include <array>

using namespace std;

Mesh::Mesh(GameObject& owner, Formats format, const void* vertex_data, unsigned int numVerts, const unsigned int* index_data, unsigned int numIndexs) :
	Component(owner),
	_format(format),
	_numVerts(numVerts),
	_numIndexs(numIndexs)
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

Mesh::Mesh(Mesh&& b) noexcept :
	Component(b.gameObject),
	_format(b._format),
	_vertex_buffer_id(b._vertex_buffer_id),
	_numVerts(b._numVerts),
	_indexs_buffer_id(b._indexs_buffer_id),
	_numIndexs(b._numIndexs)
{
	b._vertex_buffer_id = 0;
	b._indexs_buffer_id = 0;
}

Mesh::Mesh(const Mesh& cpy) : 
Component(cpy.gameObject),
meshName(cpy.meshName),
_format(cpy._format),
_vertex_buffer_id(cpy._vertex_buffer_id),
_numVerts(cpy._numVerts),
_indexs_buffer_id(cpy._indexs_buffer_id),
_numIndexs(cpy._numIndexs)
{
}

void Mesh::draw() {
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
		(texture) ? texture->bind() : texture->unbind();
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

Mesh::~Mesh() {
	if (_vertex_buffer_id) glDeleteBuffers(1, &_vertex_buffer_id);
	if (_indexs_buffer_id) glDeleteBuffers(1, &_indexs_buffer_id);
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

void Mesh::Update()
{
	draw();
}