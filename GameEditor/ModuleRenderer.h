#pragma once
#include "Module.h"
#include "Globals.h"
#include <glm/gtc/type_ptr.hpp>
#include "..\GameEngine\Physics.hpp"

class ModuleRenderer : public Module
{
public:
	ModuleRenderer(Application* app, bool start_enabled = true);
	~ModuleRenderer();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void DoCameraInput();
	void keysInputFPS();
	void mouseInputFPS();
	void mouseCamOrbit();
	void mouseCameraPan();
	void DoZoom();
	void FocusCamera();

	GameObject* DoClickRayCast();
	Ray CalculateRay();
	void DebugRay(Ray ray);
	Triangle CalculateTriangle(GameObject* triObject, int iterator);
	void CheckMeshCollisionRecursive(Ray& ray, GameObject* object, std::map<float, GameObject*>& hitObjects);

public:

	bool vsync;

private:

	double camSpeed = 0.05;
};