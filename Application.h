#pragma once

#include "p2List.h"
#include "Globals.h"
//#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
//#include "ModuleAudio.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleCamera3D* camera;
	ModuleRenderer3D* renderer3D;
	//ModuleAudio* audio;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};