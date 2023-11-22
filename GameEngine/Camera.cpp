#include "Camera.h"
#include "GameObject.h"

Camera::Camera(GameObject& owner) : Component(owner), fov(60), aspectRatio(4.0 / 3.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), camOffset(9.0)
{
	lookAtPos = vec3(0, 0, 0);
}

Camera::~Camera() {}

void Camera::Update() 
{
	lookAtPos = this->gameObject.GetComponent<Transform>()->position() + this->gameObject.GetComponent<Transform>()->forward() * camOffset;
}