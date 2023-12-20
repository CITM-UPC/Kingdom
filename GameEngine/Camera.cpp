#include "Camera.h"
#include "GameObject.h"

#include <GL/glew.h>

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

static inline void glVec3(const vec3& v) { glVertex3dv(&v.x); }

void Camera::Render()
{
	clippingPlaneViewNear;
	clippingPlaneViewFar;
	// Render Frustum

	double verticalFovDistance = sin(glm::radians(fov / 2));

	vec3 a = { clippingPlaneViewNear * -verticalFovDistance * aspectRatio, clippingPlaneViewNear * verticalFovDistance, clippingPlaneViewNear };
	vec3 b = { clippingPlaneViewNear * verticalFovDistance * aspectRatio, clippingPlaneViewNear * verticalFovDistance, clippingPlaneViewNear };
	vec3 c = { clippingPlaneViewNear * -verticalFovDistance * aspectRatio, clippingPlaneViewNear * -verticalFovDistance, clippingPlaneViewNear };
	vec3 d = { clippingPlaneViewNear * verticalFovDistance * aspectRatio, clippingPlaneViewNear * -verticalFovDistance, clippingPlaneViewNear };

	vec3 e = { clippingPlaneViewFar * -verticalFovDistance * aspectRatio, clippingPlaneViewFar * verticalFovDistance, clippingPlaneViewFar };
	vec3 f = { clippingPlaneViewFar * verticalFovDistance * aspectRatio, clippingPlaneViewFar * verticalFovDistance, clippingPlaneViewFar };
	vec3 g = { clippingPlaneViewFar * -verticalFovDistance * aspectRatio, clippingPlaneViewFar * -verticalFovDistance, clippingPlaneViewFar };
	vec3 h = { clippingPlaneViewFar * verticalFovDistance * aspectRatio, clippingPlaneViewFar * -verticalFovDistance, clippingPlaneViewFar };

	glLineWidth(2);
	glBegin(GL_LINE_STRIP);

	glVec3(a);
	glVec3(b);
	glVec3(d);
	glVec3(c);
	glVec3(a);

	glVec3(e);
	glVec3(f);
	glVec3(h);
	glVec3(g);
	glVec3(e);
	glEnd();

	glBegin(GL_LINES);
	glVec3(h);
	glVec3(d);
	glVec3(f);
	glVec3(b);
	glVec3(g);
	glVec3(c);
	glEnd();
}

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