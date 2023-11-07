#include "Camera.h"

Camera::Camera(GameObject& owner) : Component(owner), fov(60), aspectRatio(4.0 / 3.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), camOffset(3)
{
	//(Transform*)gameObject->GetComponent(Component::Type::TRANSFORM)->position = glm::dvec3(2, 2, -5);
	UpdateLookAt();
}

Camera::~Camera() {}

void Camera::UpdateLookAt()
{
	//Transform* transform = gameObject->GetComponent(Component::Type::TRANSFORM);
	//lookAtPos = transform->position + (transform->forward * camOffset);
}

void Camera::Update() {}
