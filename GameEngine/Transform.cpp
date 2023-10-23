#include "Transform.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);

	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);

	transformMatrix = glm::mat4(1.0); //Identity
	rotMatrix = glm::mat3(1.0);
}

Transform::~Transform() {}

void Transform::MoveTo(vec3 position)
{
	this->position = position;
}

void Transform::Move(vec3 displacement, Space referenceFrame)
{
	ENGINE_LOG("Move called");

	//vec3 resultingVector = transformMatrix * displacement;

	//position += resultingVector;
}

void Transform::Rotate(glm::vec3 axis)
{
	//Translate the axis to the local refernce frame to apply the rotation
	glm::mat3x3 referenceFrame;

	referenceFrame[0].x = right.x;		referenceFrame[0].y = right.y;		referenceFrame[0].z = right.z;
	referenceFrame[1].x = up.x;			referenceFrame[1].y = up.y;			referenceFrame[1].z = up.z;
	referenceFrame[2].x = forward.x;	referenceFrame[2].y = forward.y;	referenceFrame[2].z = forward.z;

	axis = referenceFrame * axis;


	//Generate the rotation matrix that corresponds to the rotation
	glm::mat3x3 rotX = glm::mat3x3(	1,	0,								0,
									0,	glm::cos(glm::radians(axis.x)), -glm::sin(glm::radians(axis.x)),
									0,	glm::sin(glm::radians(axis.x)),	glm::cos(glm::radians(axis.x)));

	glm::mat3x3 rotY = glm::mat3x3(	glm::cos(glm::radians(axis.y)), 0, glm::sin(glm::radians(axis.y)),
									0,								1, 0,
								   -glm::sin(glm::radians(axis.y)), 0, glm::cos(glm::radians(axis.y)));

	glm::mat3x3 rotZ = glm::mat3x3(	glm::cos(glm::radians(axis.z)), -glm::sin(glm::radians(axis.z)), 0,
									glm::sin(glm::radians(axis.z)),	 glm::cos(glm::radians(axis.z)), 0,
									0,								 0,								 1);

	rotMatrix = rotZ * rotY * rotX;

	//Apply the generated rotation matrix to the existing director vectors
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	up = rotMatrix * up;
}
