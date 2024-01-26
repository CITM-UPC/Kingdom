#pragma once

#include <list>
#include "Engine_Globals.h"
#include "Engine_Module.h"

#include "ScriptingEngine.h"
//#include "ModuleInput.h"
#include "Engine_ModuleRenderer3D.h"
#include "Engine_ModuleScene.h"
//#include "ModuleAudio.h"

#include <IL/il.h>
#include <random>
#include "Camera.h"
#include "pcg_random.hpp"

using UUID32 = uint32_t;

class GameEngine
{
public:

	ScriptingEngine* sEngine;

	//ModuleInput* input;
	Engine_ModuleRenderer3D* renderer3D;
	Engine_ModuleScene* scene;
	//ModuleAudio* audio;

	GameObject cameraGO;

	std::list<std::string> logHistory;

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

	std::string getDevILVersion() {
		return std::to_string(ilGetInteger(IL_VERSION_NUM));
	}

	std::string getOpenGLVersion() {
		const GLubyte* version = glGetString(GL_VERSION);
		return reinterpret_cast<const char*>(version);
	}

	// Function to generate a random 32-bit UUID using PCG
	UUID32 generateUUID32() {
		pcg32 rng(std::random_device{}());

		// Generate a random 32-bit UUID
		return rng();
	}

private:

	void AddModule(Engine_Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
