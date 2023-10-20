#include "Transform.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);

	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);

	rotMatrix = glm::mat3x3(right.x, up.x, forward.x,
							right.y, up.y, forward.y,
							right.z, up.z, forward.z);
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

	vec3 resultingVector = rotMatrix * displacement;

	position += resultingVector;
}

glm::mat3x3 Transform::EulerToMat(vec3 euler)
{
	float p = glm::radians(euler.x);  // Rotation around the X-axis (pitch)
	float y = glm::radians(euler.y);    // Rotation around the Y-axis (yaw)
	float r = glm::radians(euler.z);   // Rotation around the Z-axis (roll)

	//glm::mat4 rotationMatrix = glm::eulerAngleYXZ(pitch, yaw, roll);

	glm::mat3x3 resMat = glm::mat3x3(	cos(p) * cos(y),	cos(y) * sin(p) * sin(r) - cos(r) * sin(y),		cos(r) * cos(y) * sin(p) + sin(y) * sin(r),
										cos(p) * sin(y),	sin(y) * sin(p) * sin(r) + cos(r) * cos(y),		sin(p) * sin(y) * cos(r) - cos(y) * sin(r),
										-sin(p),			cos(p) * sin(r),								cos(p) * cos(r));
	return resMat;
}
