#pragma once

#include <chrono>
#include <thread>
#include <vector>
#include <list>
#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer.h"
#include "ModuleUI.h"
#include "ModuleResources.h"

#include "..\GameEngine\GameEngine.h"

using namespace std;
using namespace chrono;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer* renderer;
	ModuleResources* resources;
	ModuleUI* ui;

	GameEngine* gEngine;

	int targetFPS;
	duration<double> frameDurationTime;

private:

	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	vector<float> fpsHistory;
	list<string> logHistory;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};