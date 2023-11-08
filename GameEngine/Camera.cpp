#include "Camera.h"

Camera::Camera(GameObject& owner) : Component(owner), fov(60), aspectRatio(4.0 / 3.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), camOffset(9)
{
}

Camera::~Camera() {}

void Camera::Update() {}