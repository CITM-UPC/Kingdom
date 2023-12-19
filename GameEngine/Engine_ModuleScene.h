#pragma once

#include "Engine_Module.h"
#include "Engine_Globals.h"

struct Scene
{
	std::list<std::unique_ptr<GameObject>> gameObjectList;
	string name, fileName;
};

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

	void recursiveObjectRendering(GameObject* GoToRender);

	Scene currentScene;

	// Create new Scene (erases all current scene variables)
	void NewScene();
	// Saves current scene inside its file
	void SaveScene();
	// Saves current scene inside a new file
	void SaveAsScene(string fileName);
	// Loads scene from a file
	void LoadScene(string fileName);
	// Load GameObject from a file
	void LoadGameObject(json rootGOjsValue);

public:

	void addGameObject();

	void addGameObject(const std::string& filePath);

	void addGameObject(Primitive* shape);

	// Add GameObject with information from scene loading
	void addGameObject(string name, unsigned long UUID, bool active, unsigned long parentUUID = 0);

	// Only used when gameobject has no parent, removes a parent from the scene
	void removeGameObject(GameObject* GOtoDelete);

	GameObject* findGameObjectfromUUID(GameObject* parent, unsigned long UUID);

	void deleteSubstring(std::string& mainString, const std::string& substringToDelete) {
		size_t pos = mainString.find(substringToDelete);

		// Iterate until all occurrences are removed
		while (pos != std::string::npos) {
			mainString.erase(pos, substringToDelete.length());
			pos = mainString.find(substringToDelete, pos);
		}
	}

	void eraseBeforeDelimiter(std::string& str) {
		size_t found = str.find_last_of("\\/");
		if (found != std::string::npos) {
			str.erase(0, found + 1);
		}
	}

	void detectAndIncrement(std::string mainString, const std::string& substring, int& counter) {
		if (mainString.find(substring) != std::string::npos) {
			counter++;
		}
	}

	int checkNameAvailability(std::string name, GameObject* parent) {
		int count = 0;

		for (const auto& vector : parent->childs) {
			detectAndIncrement(vector->name, name, count);
		}

		return count;
	}
	int checkNameAvailability(std::string name) {
		int count = 0;

		for (const auto& vector : currentScene.gameObjectList) {
			detectAndIncrement(vector->name, name, count);
		}

		return count;
	}
private:
};