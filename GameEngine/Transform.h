#pragma once
#include "Component.h"
#include "Engine_Globals.h"
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component
{
public:
	enum class Space
	{
		LOCAL,
		GLOBAL,
	};

	Transform();
	~Transform();

	//Moves the object to 'position'.
	void MoveTo(vec3 position, Space referenceFrame = Space::GLOBAL);

	//Moves the object in 'displacement' increments.
	void Move(vec3 displacement, Space referenceFrame = Space::LOCAL); 

	//Rotates the object for its rotation to be the one given by 'axis'.
	void RotateTo(vec3f axis);

	//Rotates the object in 'axis' increments.
	void Rotate(vec3f axis, Space referenceFrame = Space::LOCAL);

	void Update() override;

public:
	vec3 position;
	vec3 rotation;

	vec3 forward;
	vec3 right;
	vec3 up;

	glm::mat3x3 referenceFrameMat;

};

