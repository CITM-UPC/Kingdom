#pragma once
#include "Engine_Globals.h"

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
	vec3 position;
	vec3 rotation;
	vec3 scale; //not used at the moment
	
	vec3 localPosition; //not used at the moment
	vec3 localRotation; //not used at the moment
	vec3 localScale; //not used at the moment

	vec3 forward;
	vec3 right;
	vec3 up;

private:
	glm::mat3x3 rotMatrix = glm::mat3x3(right.x, up.x, forward.x,
										right.y, up.y, forward.y,
										right.z, up.z, forward.z);

};

