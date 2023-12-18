#include "Engine_Globals.h"
#include "GameEngine.h"
#include "Engine_ModuleScene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

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
	for (auto& GO : gameObjectList)
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

	gameObjectList.push_back(std::move(gameObjectToAdd));

	gEngine->logHistory.push_back("[Engine] Add GameObject");
}

void Engine_ModuleScene::addGameObject(const std::string& filePath)
{
	gEngine->logHistory.push_back("[Engine] Add GameObject with path " + filePath);

	addGameObject();

	auto& gOparent = gameObjectList.back();

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

void Engine_ModuleScene::addGameObject(Primitive * shape)
{
	std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

	std::string meshName = shape->GetType();

	std::string goName = meshName;
	int currentCopies = checkNameAvailability(goName);
	if (currentCopies > 0) {
		goName.append("(" + std::to_string(currentCopies) + ")");
	}
	gameObjectToAdd->name = goName;
	gameObjectList.push_back(std::move(gameObjectToAdd));

	MeshInfo meshInfo(shape->getVertexData()->data(), shape->getVertexData()->size(), shape->getIndexData()->data(), shape->getIndexData()->size(), shape->GetNumTexCoords(), shape->GetNumNormals(), shape->GetNumFaces());
	Mesh meshToPush(gameObjectList.back().get(), meshInfo, Mesh::Formats::F_V3);

	gameObjectList.back().get()->AddComponent<Mesh>(meshToPush);
	gameObjectList.back().get()->GetComponent<Mesh>()->setName(meshName);

	gEngine->logHistory.push_back("[Engine] Mesh (" + meshName + ") loaded with " + std::to_string(meshInfo._numFaces) + " faces, "
		+ std::to_string(meshInfo._numIndexs) + " indexs, "
		+ std::to_string(meshInfo._numNormals) + " normals, "
		+ std::to_string(meshInfo._numTexCoords) + " tex coords, and "
		+ std::to_string(meshInfo._numVerts) + " vertexs.");
}

void Engine_ModuleScene::SaveScene()
{
	for (auto& gO : gameObjectList)
	{
		gO->Save();
	}
}
void Engine_ModuleScene::LoadScene() {}