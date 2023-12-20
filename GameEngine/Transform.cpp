#include "Transform.h"
#include "GameEngine.h"

Transform::Transform(GameObject* owner) : Component(owner)
{
	_transformationMatrix = mat4(1.0);
}

Transform::Transform(GameObject* owner, mat4 transmat) : Component(owner)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			_transformationMatrix[i][j] = transmat[i][j];
		}
	}

	//_transformationMatrix = transmat;
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

void Transform::Render() {}

json Transform::SaveInfo()
{
	json obj;

	obj["Owner"] = owner->UUID;
	obj["Type"] = static_cast<int>(getType());

	json transformationMatrixArray;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			transformationMatrixArray.push_back(_transformationMatrix[i][j]);
		}
	}

	obj["Transformation Matrix"] = transformationMatrixArray;

	return obj;
}