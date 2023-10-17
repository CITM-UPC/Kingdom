#pragma once

#include <list>
#include "Engine_Globals.h"
#include "Engine_Module.h"
//#include "ModuleInput.h"
#include "Engine_ModuleRenderer3D.h"
//#include "ModuleAudio.h"

#include "Camera.h"

class GameEngine
{
public:
	//ModuleInput* input;
	Engine_ModuleRenderer3D* renderer3D;
	//ModuleAudio* audio;

	Camera cam;

private:

	std::list<Engine_Module*> list_modules;

public:

	GameEngine();
	~GameEngine();

	bool Init();
	bool Start();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

private:

	void AddModule(Engine_Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

