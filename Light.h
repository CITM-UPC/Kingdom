#pragma once
#include <glm/gtc/type_ptr.hpp>

struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	glm::vec4 ambient;
	glm::vec4 diffuse;

	glm::vec3 position;

	int ref;
	bool on;
};