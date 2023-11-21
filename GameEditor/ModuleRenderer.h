#pragma once
#include "Module.h"
#include "Globals.h"
#include <glm/gtc/type_ptr.hpp>

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

public:

	bool vsync;

private:

	double camSpeed = 1.0;
};