#include "Camera.h"

Camera::Camera() : fov(60), aspectRatio(4.0 / 3.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), lookAtPos(0, 0, 0), camOffset(3)
{
	//transform.position = vec3(2, 2, 5);
	transform.position = vec3(0, 0, 4);

	glm::dvec3 vecToRotate = glm::normalize(transform.position - lookAtPos);

	transform.forward = vecToRotate;

	glm::mat3x3 rot90x = glm::mat3x3(1, 0, 0,
									0, 0, -1,
									0, 1, 0);
	transform.up = rot90x * vecToRotate;

	glm::mat3x3 rot90y = glm::mat3x3(0, 0, -1,
									0, 1, 0,
									1, 0, 0);
	transform.right = rot90y * vecToRotate;

	//transform.rotMatrix = transform.EulerToMat(vecToRotate);

	//transform.forward = transform.rotMatrix * transform.forward;
	//transform.right = transform.rotMatrix * transform.right;
	//transform.up = transform.rotMatrix * transform.up;
}

void Camera::UpdateLookAt()
{
	lookAtPos = transform.position + (-transform.forward * camOffset);
}
