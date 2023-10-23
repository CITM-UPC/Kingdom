#include "Camera.h"

Camera::Camera() : fov(60), aspectRatio(4.0 / 3.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), lookAtPos(0, 0, 0), camOffset(3)
{
	transform.position = vec3(2, 2, -5);

}

void Camera::UpdateLookAt()
{
	lookAtPos = transform.position + (transform.forward * camOffset);
}
