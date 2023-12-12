#pragma once

#include "Engine_Module.h"
#include "Engine_Globals.h"

class Engine_ModuleScene : public Engine_Module
{
public:
	Engine_ModuleScene(GameEngine* gEngine, bool start_enabled = true);
	~Engine_ModuleScene();

	bool Init();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

	void SaveScene();
	void LoadScene();

public:
	// GameObject list and its methods:
	// list<unique_ptr<GameObject>> gameObjectList;
	// addGameObject()
	// addGameObject(const std::string& filePath)
	// addGameObject(Primitive* shape)
	// -------------------------------
	// text processing functions change to UUID setting and checking:
	//
	// deleteSubstring(std::string& mainString, const std::string& substringToDelete)
	// eraseBeforeDelimiter(std::string& str)
	// detectAndIncrement(std::string mainString, const std::string& substring, int& counter)
	// checkNameAvailability
	//
	// delete all this ^

private:
};