#pragma once

#include <glm/vec3.hpp>
#include "Transform.h"

struct Camera
{
public:
	Camera();
	void UpdateLookAt();

public:
	double fov;
	double aspectRatio;
	double clippingPlaneViewNear;
	double clippingPlaneViewFar;

	Transform transform;
	vec3 lookAtPos;
	double camOffset;
};