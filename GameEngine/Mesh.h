#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Component.h"
#include "Engine_Globals.h"
#include "Graphic.h"
#include "Texture2D.h"

class Mesh : public Graphic, public Component
{
public:
	enum Formats { F_V3, F_V3C4, F_V3T2 };
	struct V3 { vec3f v; };
	struct V3C4 { vec3f v; vec4f c; };
	struct V3T2 { vec3f v; vec2f t; };

private:

	std::string meshName;
	const enum Formats _format;

	unsigned int _vertex_buffer_id;
	const unsigned int _numVerts;

	unsigned int _indexs_buffer_id;
	const unsigned int _numIndexs;

public:
	Mesh(GameObject& owner) : Component(owner), _format(Formats::F_V3), _numVerts(0), _numIndexs(0) {
	}

	Mesh(GameObject& owner, Formats format, const void* vertex_data, unsigned int numVerts, const unsigned int* indexs_data = nullptr, unsigned int numIndexs = 0);
	Mesh(Mesh&& b) noexcept; //move constructor (the one used when you call std::move)
	Mesh(const Mesh& cpy);
	void draw();
	~Mesh();

	std::string getName();
	void setName(std::string name);

	const unsigned int getNumVerts();

	const unsigned int getNumIndexs();

	void Update() override;

	Type getType() const override {
		return Type::MESH;
	}

private:

	Mesh& operator=(const Mesh&);
};
