#include "Engine_Globals.h"
#include "Engine_ModuleCamera3D.h"

Engine_ModuleCamera3D::Engine_ModuleCamera3D(GameEngine* gEngine, bool start_enabled) : Engine_Module(gEngine, start_enabled)
{
	CalculateViewMatrix();

	X = glm::vec3(1.0f, 0.0f, 0.0f);
	Y = glm::vec3(0.0f, 1.0f, 0.0f);
	Z = glm::vec3(0.0f, 0.0f, 1.0f);

	Position = glm::vec3(0.0f, 0.0f, 5.0f);
	Reference = glm::vec3(0.0f, 0.0f, 0.0f);
}

Engine_ModuleCamera3D::~Engine_ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool Engine_ModuleCamera3D::Start()
{
	ENGINE_LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool Engine_ModuleCamera3D::CleanUp()
{
	ENGINE_LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
engine_status Engine_ModuleCamera3D::Update()
{
	// Recalculate matrix -------------
	CalculateViewMatrix();

	return ENGINE_UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void Engine_ModuleCamera3D::Look(const glm::vec3& Position, const glm::vec3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(glm::vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void Engine_ModuleCamera3D::LookAt(const glm::vec3& Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(glm::vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void Engine_ModuleCamera3D::Move(const glm::vec3& Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* Engine_ModuleCamera3D::GetViewMatrix()
{
	return glm::value_ptr(ViewMatrix);
}

// -----------------------------------------------------------------
void Engine_ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = glm::mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
