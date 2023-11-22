#include "Transform.h"

Transform::Transform(GameObject& owner) : Component(owner)
{
	//position = vec3(0, 0, 0);
	_position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	scale = vec3(1, 1, 1);

	//right = vec3(1, 0, 0);
	_right = vec3(1, 0, 0);
	//up = vec3(0, 1, 0);
	_up = vec3(0, 1, 0);
	//forward = vec3(0, 0, 1);
	_forward = vec3(0, 0, 1);

	_padding1 = _padding2 = _padding3 = _padding4 = 0;

	//referenceFrameMat = glm::mat3(1.0); //Identity
}

Transform::~Transform() {}

void Transform::MoveTo(vec3 position, Space referenceFrame)
{
	/*vec3 resultingVector = position;

	if (referenceFrame == Space::LOCAL)
	{
		resultingVector = referenceFrameMat * position;
		this->position += resultingVector;
		return;
	}

	this->position = resultingVector;*/

	_position = position;
}

void Transform::Move(vec3 displacement, Space referenceFrame)
{
	/*vec3 resultingVector = displacement;

	if (referenceFrame == Space::LOCAL)
		resultingVector = referenceFrameMat * displacement;

	position += resultingVector;*/

	glm::translate(_transformationMatrix, displacement);
}

void Transform::RotateTo(vec3f axis)
{
	/*
	//Reset the referenceFrame to world default
	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);

	//Generate the rotation matrix that corresponds to the rotation
	glm::mat3x3 rotX = glm::mat3x3(1, 0, 0,
		0, glm::cos(glm::radians(axis.x)), -glm::sin(glm::radians(axis.x)),
		0, glm::sin(glm::radians(axis.x)), glm::cos(glm::radians(axis.x)));

	glm::mat3x3 rotY = glm::mat3x3(glm::cos(glm::radians(axis.y)), 0, glm::sin(glm::radians(axis.y)),
		0, 1, 0,
		-glm::sin(glm::radians(axis.y)), 0, glm::cos(glm::radians(axis.y)));

	glm::mat3x3 rotZ = glm::mat3x3(glm::cos(glm::radians(axis.z)), -glm::sin(glm::radians(axis.z)), 0,
		glm::sin(glm::radians(axis.z)), glm::cos(glm::radians(axis.z)), 0,
		0, 0, 1);

	glm::mat3x3 rotMatrix = rotZ * rotY * rotX;

	//Apply the generated rotation matrix to the existing director vectors
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	up = rotMatrix * up;

	referenceFrameMat[0].x = right.x;		referenceFrameMat[0].y = right.y;		referenceFrameMat[0].z = right.z;
	referenceFrameMat[1].x = up.x;			referenceFrameMat[1].y = up.y;			referenceFrameMat[1].z = up.z;
	referenceFrameMat[2].x = forward.x;		referenceFrameMat[2].y = forward.y;		referenceFrameMat[2].z = forward.z;
	*/

	_right = vec3(1, 0, 0);
	_up = vec3(0, 1, 0);
	_forward = vec3(0, 0, 1);

	mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(axis.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(axis.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(axis.z), glm::vec3(0.0f, 0.0f, 1.0f));

	mat4 rotationMat = rotZ * rotY * rotX;

	_transformationMatrix = rotationMat * _transformationMatrix;
}

void Transform::Rotate(vec3 axis, Space referenceFrame)
{
	/*
	//Translate the axis to the local refernce frame to apply the rotation

	referenceFrameMat[0].x = right.x;		referenceFrameMat[0].y = right.y;		referenceFrameMat[0].z = right.z;
	referenceFrameMat[1].x = up.x;			referenceFrameMat[1].y = up.y;			referenceFrameMat[1].z = up.z;
	referenceFrameMat[2].x = forward.x;		referenceFrameMat[2].y = forward.y;		referenceFrameMat[2].z = forward.z;

	glm::vec3 vecInRefFrame = axis;
	if (referenceFrame == Space::LOCAL)
		vecInRefFrame = referenceFrameMat * axis;

	//Generate the rotation matrix that corresponds to the rotation
	glm::mat3x3 rotX = glm::mat3x3(1, 0, 0,
		0, glm::cos(glm::radians(vecInRefFrame.x)), -glm::sin(glm::radians(vecInRefFrame.x)),
		0, glm::sin(glm::radians(vecInRefFrame.x)), glm::cos(glm::radians(vecInRefFrame.x)));

	glm::mat3x3 rotY = glm::mat3x3(glm::cos(glm::radians(vecInRefFrame.y)), 0, glm::sin(glm::radians(vecInRefFrame.y)),
		0, 1, 0,
		-glm::sin(glm::radians(vecInRefFrame.y)), 0, glm::cos(glm::radians(vecInRefFrame.y)));

	glm::mat3x3 rotZ = glm::mat3x3(glm::cos(glm::radians(vecInRefFrame.z)), -glm::sin(glm::radians(vecInRefFrame.z)), 0,
		glm::sin(glm::radians(vecInRefFrame.z)), glm::cos(glm::radians(vecInRefFrame.z)), 0,
		0, 0, 1);

	glm::mat3x3 rotMatrix = rotZ * rotY * rotX;

	//Apply the generated rotation matrix to the existing director vectors
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	up = rotMatrix * up;
	*/

	glm::vec4 vecInRefFrame = { axis.x, axis.y, axis.z, 0 };

	if (referenceFrame == Space::LOCAL)
		vecInRefFrame = _transformationMatrix * vecInRefFrame;

	mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(vecInRefFrame.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(vecInRefFrame.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(vecInRefFrame.z), glm::vec3(0.0f, 0.0f, 1.0f));

	mat4 rotationMat = rotZ * rotY * rotX;

	_transformationMatrix = rotationMat * _transformationMatrix;
}

void Transform::Update() {}