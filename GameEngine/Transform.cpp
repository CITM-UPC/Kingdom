#include "Transform.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);

	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);
}

Transform::~Transform() {}

void Transform::MoveTo(vec3 position)
{
	this->position = position;
}

void Transform::Move(vec3 displacement, Space referenceFrame)
{
	/*vec3 directorVector = App->gEngine->cam.lookAtPos - App->gEngine->cam.transform.position;
	forward = glm::normalize(directorVector) * increase;

	glm::mat4x4()*/

	ENGINE_LOG("Move called");

	position += rotMatrix * displacement;
}
