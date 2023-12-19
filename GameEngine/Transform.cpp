#include "Transform.h"
#include "GameEngine.h"

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

	obj["Owner"] = std::to_string(this->owner->UUID).c_str();
	obj["Type"] = static_cast<int>(getType());

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			obj["Transformation Matrix"].append(_transformationMatrix[i][j]);
		}
	}

	obj["Right"].append(_right.x);
	obj["Right"].append(_right.y);
	obj["Right"].append(_right.z);

	obj["Up"].append(_up.x);
	obj["Up"].append(_up.y);
	obj["Up"].append(_up.z);

	obj["Forward"].append(_forward.x);
	obj["Forward"].append(_forward.y);
	obj["Forward"].append(_forward.z);

	obj["Position"].append(_position.x);
	obj["Position"].append(_position.y);
	obj["Position"].append(_position.z);

	return obj;
}