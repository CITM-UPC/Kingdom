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

public:
	glm::mat3x3 EulerToMat(vec3 euler);

public:
	vec3 position;
	vec3 rotation;
	vec3 scale; //not used at the moment
	
	vec3 localPosition; //not used at the moment
	vec3 localRotation; //not used at the moment
	vec3 localScale; //not used at the moment

	vec3 forward;
	vec3 right;
	vec3 up;

public:
	glm::mat3x3 rotMatrix;

};

