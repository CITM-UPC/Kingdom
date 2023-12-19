#include "Engine_Globals.h"
#include "GameEngine.h"
#include "Engine_ModuleScene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "JSONParser.h"

namespace fs = std::filesystem;

Engine_ModuleScene::Engine_ModuleScene(GameEngine* gEngine, bool start_enabled) : Engine_Module(gEngine, start_enabled)
{
}

// Destructor
Engine_ModuleScene::~Engine_ModuleScene() = default;

bool Engine_ModuleScene::Init()
{
	fs::create_directories("Library/Meshes/");
	fs::create_directories("Library/Materials/");

	addGameObject("Assets/BakerHouse.fbx");

	return true;
}

engine_status Engine_ModuleScene::PreUpdate() { return ENGINE_UPDATE_CONTINUE; }

engine_status Engine_ModuleScene::Update() { return ENGINE_UPDATE_CONTINUE; }

engine_status Engine_ModuleScene::PostUpdate()
{
	for (auto& GO : currentScene.gameObjectList)
	{
		recursiveObjectRendering(GO.get());
	}

	return ENGINE_UPDATE_CONTINUE;
}

bool Engine_ModuleScene::CleanUp() { return true; }

void Engine_ModuleScene::recursiveObjectRendering(GameObject* GoToRender)
{
	GoToRender->UpdateComponents();

	if (!GoToRender->childs.empty())
	{
		for (auto& child : GoToRender->childs)
		{
			recursiveObjectRendering(child.get());
		}
	}
}

void Engine_ModuleScene::addGameObject()
{
	std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

	std::string meshName = "GameObject";
	int currentCopies = checkNameAvailability(meshName);
	if (currentCopies > 0) {
		meshName.append("(" + std::to_string(currentCopies) + ")");
	}
	gameObjectToAdd->name = meshName;

	gameObjectToAdd->UUID = gEngine->generateUUID32();

	currentScene.gameObjectList.push_back(std::move(gameObjectToAdd));

	gEngine->logHistory.push_back("[Engine] Add GameObject");
}

void Engine_ModuleScene::addGameObject(const std::string& filePath)
{
	gEngine->logHistory.push_back("[Engine] Add GameObject with path " + filePath);

	addGameObject();

	auto& gOparent = currentScene.gameObjectList.back();

	auto meshInfo_vector = MeshLoader::loadMeshFromFile(filePath);
	auto texture_paths_vector = MeshLoader::loadTextureFromFile(filePath);

	int i = 0;
	for (auto& meshInfo : meshInfo_vector)
	{
		std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

		std::string fileName = filePath;
		eraseBeforeDelimiter(fileName);
		std::string meshName = fileName;
		deleteSubstring(meshName, ".fbx");

		int currentCopies = checkNameAvailability(meshName, gOparent.get());
		if (currentCopies > 0) {
			meshName.append("(" + std::to_string(currentCopies) + ")");
		}
		gameObjectToAdd->name = meshName;
		gameObjectToAdd->UUID = gEngine->generateUUID32();

		gameObjectToAdd->parent = gOparent.get();

		gOparent->childs.push_back(std::move(gameObjectToAdd));

		string folderName = "Library/Meshes/";

		ofstream oFile(folderName + meshName + ".mesh", ios::binary);
		oFile << meshInfo;
		oFile.close();

		Texture2D textureToPush(gOparent->childs.back().get(), texture_paths_vector.at(i));
		gOparent->childs.back().get()->AddComponent<Texture2D>(textureToPush);

		Mesh meshToPush(gOparent->childs.back().get(), meshInfo, Mesh::Formats::F_V3T2);
		gOparent->childs.back().get()->AddComponent<Mesh>(meshToPush);

		gOparent->childs.back().get()->GetComponent<Mesh>()->setName(fileName);
		gOparent->childs.back().get()->GetComponent<Mesh>()->texture = gOparent->childs.back().get()->GetComponent<Texture2D>();
		i++;

		gEngine->logHistory.push_back("[Engine] Mesh loaded with " + std::to_string(meshInfo._numFaces) + " faces, "
			+ std::to_string(meshInfo._numIndexs) + " indexs, "
			+ std::to_string(meshInfo._numNormals) + " normals, "
			+ std::to_string(meshInfo._numTexCoords) + " tex coords, and "
			+ std::to_string(meshInfo._numVerts) + " vertexs.");
	}
}

void Engine_ModuleScene::addGameObject(Primitive* shape)
{
	std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

	std::string meshName = shape->GetType();

	std::string goName = meshName;
	int currentCopies = checkNameAvailability(goName);
	if (currentCopies > 0) {
		goName.append("(" + std::to_string(currentCopies) + ")");
	}
	gameObjectToAdd->name = goName;
	currentScene.gameObjectList.push_back(std::move(gameObjectToAdd));

	MeshInfo meshInfo(shape->getVertexData()->data(), shape->getVertexData()->size(), shape->getIndexData()->data(), shape->getIndexData()->size(), shape->GetNumTexCoords(), shape->GetNumNormals(), shape->GetNumFaces());
	Mesh meshToPush(currentScene.gameObjectList.back().get(), meshInfo, Mesh::Formats::F_V3);

	currentScene.gameObjectList.back().get()->AddComponent<Mesh>(meshToPush);
	currentScene.gameObjectList.back().get()->GetComponent<Mesh>()->setName(meshName);

	gEngine->logHistory.push_back("[Engine] Mesh (" + meshName + ") loaded with " + std::to_string(meshInfo._numFaces) + " faces, "
		+ std::to_string(meshInfo._numIndexs) + " indexs, "
		+ std::to_string(meshInfo._numNormals) + " normals, "
		+ std::to_string(meshInfo._numTexCoords) + " tex coords, and "
		+ std::to_string(meshInfo._numVerts) + " vertexs.");
}

void Engine_ModuleScene::removeGameObject(GameObject* GOtoDelete)
{
	auto it = std::find_if(currentScene.gameObjectList.begin(), currentScene.gameObjectList.end(), [GOtoDelete](const std::unique_ptr<GameObject>& GO) {
		return GO.get() == GOtoDelete;
		});

	if (it != currentScene.gameObjectList.end())
	{
		currentScene.gameObjectList.erase(it);
	}
}

void Engine_ModuleScene::NewScene()
{
	currentScene.fileName = "";
	currentScene.name = "";
	currentScene.gameObjectList.clear();
}

void Engine_ModuleScene::SaveScene()
{
	Json::Value sceneValue;

	for (auto& go : currentScene.gameObjectList)
	{
		sceneValue["GameObject List"].append(go.get()->SaveInfo());
	}

	ofstream("Assets/" + currentScene.fileName) << sceneValue;

	gEngine->logHistory.push_back("[Engine] Scene file with name: " + currentScene.fileName + " saved");
}

void Engine_ModuleScene::SaveAsScene(string fileName)
{
	// Create scene file, save and set as current scene file

	Json::Value sceneValue;

	for (auto& go : currentScene.gameObjectList)
	{
		sceneValue["GameObject List"].append(go.get()->SaveInfo());
	}

	currentScene.name = fileName;
	currentScene.fileName = fileName + ".mdng";

	ofstream("Assets/" + currentScene.fileName) << sceneValue;

	gEngine->logHistory.push_back("[Engine] Scene file with name: " + fileName + " created");
}

void Engine_ModuleScene::LoadScene(string fileName)
{
	currentScene.gameObjectList.clear();

	Json::Value sceneToLoad = GetJsonFile(currentScene.fileName);
}