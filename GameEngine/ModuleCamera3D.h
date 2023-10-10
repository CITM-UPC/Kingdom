#pragma once
#include "Module.h"
#include <glm/gtc/type_ptr.hpp>

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(GameEngine* gEngine, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Look(const glm::vec3 &Position, const glm::vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const glm::vec3 &Spot);
	void Move(const glm::vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	glm::vec3 X, Y, Z, Position, Reference;

private:

	glm::mat4x4 ViewMatrix, ViewMatrixInverse;
};