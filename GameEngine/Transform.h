#pragma once
#include "Engine_Globals.h"
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
	enum class Space
	{
		LOCAL,
		GLOBAL,
	};

public:
	Transform();
	~Transform();

	//Moves to 'position' in the world space.
	void MoveTo(vec3 position);	

	//Moves 'displacement' amount in 'referenceFrame' coordinates.
	void Move(vec3 displacement, Space referenceFrame = Space::LOCAL); 

	void Rotate(glm::vec3 axis, float angle);

	void UpdateValues();

public:
	vec3 position;
	vec3 rotation;

	vec3 forward;
	vec3 right;
	vec3 up;

	glm::mat4 transformMatrix;
	glm::mat3x3 rotMatrix;

};

