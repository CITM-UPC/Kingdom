#pragma once

#include <list>
#include "Globals.h"
#include "Module.h"
//#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
//#include "ModuleAudio.h"

class GameEngine
{
public:
	//ModuleInput* input;
	ModuleCamera3D* camera;
	ModuleRenderer3D* renderer3D;
	//ModuleAudio* audio;

private:

	std::list<Module*> list_modules;

public:

	GameEngine();
	~GameEngine();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

