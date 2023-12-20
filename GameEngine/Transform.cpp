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

json Transform::SaveInfo()
{
	json obj;

	obj["Owner"] = owner->UUID;
	obj["Type"] = static_cast<int>(getType());

	json transformationMatrixArray;
	json rightArray;
	json upArray;
	json forwardArray;
	json positionArray;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			transformationMatrixArray.push_back(_transformationMatrix[i][j]);
		}
	}

	rightArray.push_back(_right.x);
	rightArray.push_back(_right.y);
	rightArray.push_back(_right.z);

	upArray.push_back(_up.x);
	upArray.push_back(_up.y);
	upArray.push_back(_up.z);

	forwardArray.push_back(_forward.x);
	forwardArray.push_back(_forward.y);
	forwardArray.push_back(_forward.z);

	positionArray.push_back(_position.x);
	positionArray.push_back(_position.y);
	positionArray.push_back(_position.z);

	obj["Transformation Matrix"] = transformationMatrixArray;
	obj["Right"] = rightArray;
	obj["Up"] = upArray;
	obj["Forward"] = forwardArray;
	obj["Position"] = positionArray;

	return obj;
}