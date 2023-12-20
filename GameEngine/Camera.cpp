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

Camera::Camera(GameObject* owner, double ifov, double ratio, double clipnear, double clipfar, double camoffset, glm::dvec3 lookatpos) :
	Component(owner),
	fov(ifov),
	aspectRatio(ratio),
	clippingPlaneViewNear(clipnear),
	clippingPlaneViewFar(clipfar),
	camOffset(camoffset),
	lookAtPos(lookatpos)
{
}

Camera::~Camera() {}

void Camera::Update()
{
	//lookAtPos = this->owner->GetComponent<Transform>()->position() + this->owner->GetComponent<Transform>()->forward() * camOffset;
}

void Camera::Render() {}

json Camera::SaveInfo()
{
	json obj;

	obj["Owner"] = obj["Owner"] = std::to_string(owner->UUID);

	obj["Fov"] = fov;
	obj["Aspect Ratio"] = aspectRatio;
	obj["Clipping Plane View Near"] = clippingPlaneViewNear;
	obj["Clipping Plane View Far"] = clippingPlaneViewFar;

	obj["Camera Offset"] = camOffset;

	return obj;
}