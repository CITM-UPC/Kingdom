#include "Transform.h"

Transform::Transform(GameObject* owner) : Component(owner)
{
	_transformationMatrix = mat4(1.0);
}

Transform::~Transform() {}

void Transform::MoveTo(vec3 position)
{
	_position = position;
}

void Transform::Move(vec3 displacement, Space referenceFrame)
{
	glm::mat3 referenceFrameMat = (glm::mat4)_transformationMatrix;

	vec3 vecInRefFrame = displacement;
	if (referenceFrame == Space::GLOBAL)
		vecInRefFrame = displacement * referenceFrameMat;

	_transformationMatrix = glm::translate(_transformationMatrix, vecInRefFrame);
}

void Transform::RotateTo(double angle, vec3 axis)
{
	vec3 normalizedVec = glm::normalize(axis);

	_right = vec3(1, 0, 0);
	_up = vec3(0, 1, 0);
	_forward = vec3(0, 0, 1);

	_transformationMatrix = glm::rotate(_transformationMatrix, glm::radians(angle), normalizedVec);
}

void Transform::Rotate(double angle, vec3 axis, Space referenceFrame)
{
	glm::mat3 referenceFrameMat = (glm::mat4)_transformationMatrix;

	vec3 vecInRefFrame = glm::normalize(axis);
	if (referenceFrame == Space::GLOBAL)
		vecInRefFrame = axis * referenceFrameMat;

	_transformationMatrix = glm::rotate(_transformationMatrix, glm::radians(angle), vecInRefFrame);
}

void Transform::Update() {}

Json::Value Transform::SaveInfo()
{
	Json::Value obj;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			obj["Transform"]["Transformation Matrix"].append(_transformationMatrix[i][j]);
		}
	}

	obj["Transform"]["Right"].append(_right.x);
	obj["Transform"]["Right"].append(_right.y);
	obj["Transform"]["Right"].append(_right.z);

	obj["Transform"]["Up"].append(_up.x);
	obj["Transform"]["Up"].append(_up.y);
	obj["Transform"]["Up"].append(_up.z);

	obj["Transform"]["Forward"].append(_forward.x);
	obj["Transform"]["Forward"].append(_forward.y);
	obj["Transform"]["Forward"].append(_forward.z);

	obj["Transform"]["Position"].append(_position.x);
	obj["Transform"]["Position"].append(_position.y);
	obj["Transform"]["Position"].append(_position.z);

	obj["Transform"]["Padding 1"] = _padding1;
	obj["Transform"]["Padding 2"] = _padding2;
	obj["Transform"]["Padding 3"] = _padding3;
	obj["Transform"]["Padding 4"] = _padding4;

	return obj;
}