#include "Engine_Globals.h"
#include "GameEngine.h"
#include "Engine_ModuleScene.h"
#include <iostream>

Engine_ModuleScene::Engine_ModuleScene(GameEngine* gEngine, bool start_enabled) : Engine_Module(gEngine, start_enabled)
{
}

// Destructor
Engine_ModuleScene::~Engine_ModuleScene() = default;

bool Engine_ModuleScene::Init()
{
	return true;
}

engine_status Engine_ModuleScene::PreUpdate() { return ENGINE_UPDATE_CONTINUE; }
engine_status Engine_ModuleScene::Update() { return ENGINE_UPDATE_CONTINUE; }
engine_status Engine_ModuleScene::PostUpdate() { return ENGINE_UPDATE_CONTINUE; }

bool Engine_ModuleScene::CleanUp() { return true; }

void Engine_ModuleScene::SaveScene() {}
void Engine_ModuleScene::LoadScene() {}