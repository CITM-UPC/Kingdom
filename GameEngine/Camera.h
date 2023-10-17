#pragma once

#include <glm/vec3.hpp>

struct Camera
{
	Camera();

	double fov;
	double aspectRatio;
	double clippingPlaneViewNear;
	double clippingPlaneViewFar;

	glm::dvec3 camCenter;
	glm::dvec3 lookAtPos;
	glm::dvec3 upVector;
};