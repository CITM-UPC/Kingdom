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

#include "..\GameEngine\GameEngine.h"

using namespace std;
using namespace chrono;

static const unsigned int targetFPS = 60;
static const auto frameDurationTime = 1.0s / targetFPS;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer* renderer;
	ModuleUI* ui;

	GameEngine* gEngine;

private:

	std::list<Module*> list_modules;
	

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	vector<float> fpsHistory;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};