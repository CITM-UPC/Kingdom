#pragma once

#include <glm/vec3.hpp>
#include "Component.h"

class Camera : public Component
{
public:

	Camera(GameObject* owner);
	Camera(const Camera& other);
	~Camera();

	void Update() override;
	Type getType() const override {
		return Type::CAMERA;
	}

	json SaveInfo();

public:

	double fov;
	double aspectRatio;
	double clippingPlaneViewNear;
	double clippingPlaneViewFar;

	glm::dvec3 lookAtPos;
	double camOffset;
};