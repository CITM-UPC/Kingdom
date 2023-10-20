#pragma once

#include <glm/vec3.hpp>
#include "Transform.h"

struct Camera
{
	Camera();

	double fov;
	double aspectRatio;
	double clippingPlaneViewNear;
	double clippingPlaneViewFar;

	void UpdateLookAt();

	Transform transform;
	vec3 lookAtPos;
	double camOffset;
};