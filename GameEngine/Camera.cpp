#include "Camera.h"
#include "GameObject.h"

Camera::Camera(GameObject* owner) : Component(owner), fov(60), aspectRatio(4.0 / 3.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), camOffset(9.0)
{
	lookAtPos = vec3(0, 0, 0);
}

Camera::Camera(const Camera& other)
	: Component(other.owner),
	fov(other.fov),
	aspectRatio(other.aspectRatio),
	clippingPlaneViewNear(other.clippingPlaneViewNear),
	clippingPlaneViewFar(other.clippingPlaneViewFar),
	lookAtPos(other.lookAtPos),
	camOffset(other.camOffset)
{
}

Camera::~Camera() {}

void Camera::Update()
{
	lookAtPos = this->owner->GetComponent<Transform>()->position() + this->owner->GetComponent<Transform>()->forward() * camOffset;
}

Json::Value Camera::SaveInfo()
{
	Json::Value obj;

	obj["Camera"]["Fov"] = fov;
	obj["Camera"]["Aspect Ratio"] = aspectRatio;
	obj["Camera"]["Clipping Plane View Near"] = clippingPlaneViewNear;
	obj["Camera"]["Clipping Plane View Far"] = clippingPlaneViewFar;
	obj["Camera"]["Look At Position"].append(lookAtPos.x);
	obj["Camera"]["Look At Position"].append(lookAtPos.y);
	obj["Camera"]["Look At Position"].append(lookAtPos.z);
	obj["Camera"]["Camera Offset"] = camOffset;

	return obj;
}