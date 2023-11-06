#pragma once

#include <list>
#include "Engine_Globals.h"
#include "Engine_Module.h"
//#include "ModuleInput.h"
#include "Engine_ModuleRenderer3D.h"
//#include "ModuleAudio.h"
#include <IL/il.h>

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

	std::string getDevILVersion() {
		return std::to_string(ilGetInteger(IL_VERSION_NUM));
	}

	std::string getOpenGLVersion() {
		const GLubyte* version = glGetString(GL_VERSION);
		return reinterpret_cast<const char*>(version);
	}

private:

	void AddModule(Engine_Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
