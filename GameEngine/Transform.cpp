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

void Transform::Rotate(glm::vec3 axis, float angle)
{
	//transformMatrix = glm::rotate(transformMatrix, angle, axis);
	//UpdateValues();

	glm::mat3x3 rotX = glm::mat3x3(	1,	0,								0,
									0,	glm::cos(glm::radians(angle)), -glm::sin(glm::radians(angle)),
									0,	glm::sin(glm::radians(angle)),	glm::cos(glm::radians(angle)));

	glm::mat3x3 rotY = glm::mat3x3(	glm::cos(glm::radians(angle)),	0,	glm::sin(glm::radians(angle)),
									0,								1,	0,
									-glm::sin(glm::radians(angle)), 0,	glm::cos(glm::radians(angle)));

	glm::mat3x3 rotZ = glm::mat3x3(	glm::cos(glm::radians(angle)), -glm::sin(glm::radians(angle)),	0,
									glm::sin(glm::radians(angle)),	glm::cos(glm::radians(angle)),	0,
									0,								0,								1);
	forward = rotX * forward;
	up = rotX * up;


	forward = rotY * forward;
	right = rotY * right;

	up = rotZ * up;
	right = rotZ * right;


	//Setting the matrix
	rotMatrix[0].x = right.x;	rotMatrix[0].y = right.y;	rotMatrix[0].z = right.z;
	rotMatrix[1].x = forward.x;	rotMatrix[1].y = forward.y;	rotMatrix[1].z = forward.z;
	rotMatrix[2].x = up.x;		rotMatrix[2].y = up.y;		rotMatrix[2].z = up.z;
}

void Transform::UpdateValues()
{
	rotMatrix[0][0] = transformMatrix[0][0];	rotMatrix[0][1] = transformMatrix[0][1];	rotMatrix[0][2] = transformMatrix[0][2];
	rotMatrix[1][0] = transformMatrix[1][0];	rotMatrix[1][1] = transformMatrix[1][1];	rotMatrix[1][2] = transformMatrix[1][2];
	rotMatrix[2][0] = transformMatrix[2][0];	rotMatrix[2][1] = transformMatrix[2][1];	rotMatrix[2][2] = transformMatrix[2][2];
	
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	//up = rotMatrix * up;
}
