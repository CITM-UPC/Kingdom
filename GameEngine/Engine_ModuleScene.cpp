#include "Engine_Globals.h"
#include "GameEngine.h"
#include "Engine_ModuleScene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

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

engine_status Engine_ModuleScene::Update()
{
	if (!paused)
	{
		for (auto& GO : currentScene.gameObjectList)
		{
			recursiveObjectUpdate(GO.get());
		}

		if (step)
		{
			step = false;
			paused = true;
		}
	}

	if (step && paused) paused = false;

	return ENGINE_UPDATE_CONTINUE;
}

engine_status Engine_ModuleScene::PostUpdate()
{
	for (auto& GO : currentScene.gameObjectList)
	{
		recursiveObjectRendering(GO.get());
	}

	return ENGINE_UPDATE_CONTINUE;
}

bool Engine_ModuleScene::CleanUp() { return true; }

void Engine_ModuleScene::recursiveObjectUpdate(GameObject* GoToUpdate)
{
	GoToUpdate->UpdateComponents();
	if (bboxDebug) GoToUpdate->drawAABBox(GoToUpdate->computeAABB());

	if (!GoToUpdate->childs.empty())
	{
		for (auto& child : GoToUpdate->childs)
		{
			recursiveObjectRendering(child.get());
		}
	}
}

void Engine_ModuleScene::recursiveObjectRendering(GameObject* GoToRender)
{
	GoToRender->RenderComponents();
	if (bboxDebug) GoToRender->drawAABBox(GoToRender->computeAABB());

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

		gEngine->logHistory.push_back("[Engine] Mesh file created as " + meshName + ".mesh in " + folderName);

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
	json sceneValue;

	json gameobjectArray;

	for (auto& go : currentScene.gameObjectList)
	{
		//gameobjectArray.push_back(go.get()->SaveInfo());
		gameobjectArray.push_back(go.get()->UUID);
	}

	sceneValue["Game Object List"] = gameobjectArray;

	ofstream("Assets/" + currentScene.fileName) << sceneValue;

	gEngine->logHistory.push_back("[Engine] Scene file with name: " + currentScene.fileName + " saved");
}

void Engine_ModuleScene::SaveAsScene(string fileName)
{
	// Create scene file, save and set as current scene file

	json sceneValue;

	sceneValue["Name"] = fileName.c_str();

	json gameobjectArray;

	for (auto& go : currentScene.gameObjectList)
	{
		gameobjectArray.push_back(go.get()->SaveInfo());
	}

	sceneValue["Game Object List"] = gameobjectArray;

	currentScene.name = fileName;
	currentScene.fileName = fileName + ".mdng";

	ofstream("Assets/" + currentScene.fileName) << sceneValue;

	gEngine->logHistory.push_back("[Engine] Scene file with name: " + fileName + " created");
}

void Engine_ModuleScene::LoadScene(string fileName)
{
	std::ifstream file("Assets/" + fileName + ".mdng");

	json sceneToLoad;
	bool parsed = true;

	// Check if parsed correctly
	try {
		sceneToLoad = json::parse(file);
	}
	catch (json::parse_error e)
	{
		//gEngine->logHistory.push_back(e.what());
		parsed = false;
	}

	if (parsed)
	{
		NewScene();

		currentScene.name = sceneToLoad["Name"];
		currentScene.fileName = fileName;

		const json gameObjectList = sceneToLoad["Game Object List"];

		for (auto rootGO : gameObjectList)
		{
			CreateRootGOs(rootGO);
		}
	}
	else
		gEngine->logHistory.push_back("[Engine] Could not parse file");
}

void Engine_ModuleScene::CreateRootGOs(json rootGOjsValue)
{
	string name = rootGOjsValue["Name"];

	unsigned long UUID = rootGOjsValue["UUID"];

	bool isActive = rootGOjsValue["Active"];

	LoadRootGameObject(name, UUID, isActive);

	json childArray = rootGOjsValue["Childs"];

	for (auto childroot : childArray)
	{
		LoadChildGameObjectfromjson(childroot);
	}

	json componentArray = rootGOjsValue["Components"];

	for (auto comp : componentArray)
	{
		LoadComponentfromjson(comp);
	}
}

void Engine_ModuleScene::LoadRootGameObject(string name, unsigned long UUID, bool active)
{
	std::unique_ptr<GameObject> newRootGO = std::make_unique<GameObject>();

	currentScene.gameObjectList.push_back(std::move(newRootGO));

	currentScene.gameObjectList.back()->name = name;
	currentScene.gameObjectList.back()->UUID = UUID;
	currentScene.gameObjectList.back()->isActive = active;
}

void Engine_ModuleScene::LoadChildGameObjectfromjson(json parentRoot)
{
	string name = parentRoot["Name"];
	parentRoot["Active"];

	unsigned long UUID = parentRoot["UUID"];

	unsigned long parentUUID = parentRoot["Parent UUID"];

	GameObject* parent = nullptr;

	for (auto& root : currentScene.gameObjectList)
	{
		if (root.get()->UUID == parentUUID)
		{
			parent = root.get();
		}
		else
		{
			GameObject* tempParent = findGameObjectfromUUID(root.get(), parentUUID);

			if (tempParent != nullptr)
			{
				parent = tempParent;
				break;
			}
		}
	}

	std::unique_ptr<GameObject> newRootGO = std::make_unique<GameObject>();

	parent->childs.push_back(std::move(newRootGO));

	parent->childs.back()->name = name;
	parent->childs.back()->UUID = UUID;
	parent->childs.back()->parent = parent;

	json childArray = parentRoot["Childs"];

	for (auto childroot : childArray)
	{
		LoadChildGameObjectfromjson(childroot);
	}

	json componentArray = parentRoot["Components"];
	for (auto comp : componentArray)
	{
		LoadComponentfromjson(comp);
	}

	if (parent->childs.back().get()->GetComponent<Mesh>() != nullptr && parent->childs.back().get()->GetComponent<Texture2D>() != nullptr)
	{
		//newRootGO->GetComponent<Mesh>()->setName(fileName);
		parent->childs.back().get()->GetComponent<Mesh>()->texture = parent->childs.back().get()->GetComponent<Texture2D>();
	}
}

void Engine_ModuleScene::LoadComponentfromjson(json parentRoot)
{
	int itype = parentRoot["Type"];

	string path = "";

	if (parentRoot.contains("Binary Path"))	path = parentRoot["Binary Path"];

	unsigned long ownerUUID = parentRoot["Owner"];
	GameObject* owner = nullptr;
	for (auto& root : currentScene.gameObjectList)
	{
		if (root.get()->UUID == ownerUUID)
		{
			owner = root.get();
		}
		else
		{
			GameObject* tempParent = findGameObjectfromUUID(root.get(), ownerUUID);

			if (tempParent != nullptr)
			{
				owner = tempParent;
				break;
			}
		}
	}

	switch (itype)
	{
	case 0:
		// Call transform constructor with the node / modify transform information
		LoadComponentTransform(owner, parentRoot);
		break;
	case 1:
		// Call mesh constructor with the file path
		LoadComponentMesh(owner, path);
		break;
	case 2:
		// Call texture constructor with the node
		LoadComponentTexture(owner, path);
		break;
	case 3:
		// Call mesh constructor with the file path
		LoadComponentCamera(owner, parentRoot);
		break;
	default:
		gEngine->logHistory.push_back("[Engine] Could not load component: no type provided by save");

		break;
	}
}

GameObject* Engine_ModuleScene::findGameObjectfromUUID(GameObject* head, unsigned long UUIDtocompare)
{
	for (auto& child : head->childs)
	{
		if (child.get()->UUID == UUIDtocompare)
		{
			return child.get();
		}

		GameObject* tempParent = findGameObjectfromUUID(child.get(), UUIDtocompare);
		if (tempParent != nullptr)
		{
			return tempParent;
		}
	}

	return nullptr;
}

void Engine_ModuleScene::LoadComponentMesh(GameObject* owner, string path)
{
	MeshInfo newinfo = MeshInfo();

	std::ifstream meshfile(path, ios::binary);

	if (meshfile.is_open())
	{
		meshfile >> newinfo;

		Mesh newMesh(owner, newinfo, Mesh::Formats::F_V3T2);

		owner->AddComponent<Mesh>(newMesh);
	}
	else
		gEngine->logHistory.push_back("Mesh Binary File could not be open");

	meshfile.close();
}

void Engine_ModuleScene::LoadComponentTexture(GameObject* owner, string path)
{
	size_t lastOf = path.find_last_of('/');
	std::string fileName = path.substr(lastOf + 1);
	Texture2D textureToPush(owner, fileName);
	owner->AddComponent<Texture2D>(textureToPush);
}

void Engine_ModuleScene::LoadComponentTransform(GameObject* owner, json transformjsonRoot)
{
	mat4 transmatToLoad = mat4(0);

	json transmatArray = transformjsonRoot["Transformation Matrix"];

	int it = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			transmatToLoad[i][j] = transmatArray[it];
			it++;
		}
	}

	// Remove Transformation component since it is created by default on every object
	owner->RemoveComponent(Component::Type::TRANSFORM);

	Transform newtransform(owner, transmatToLoad);
	owner->AddComponent<Transform>(newtransform);
}

void Engine_ModuleScene::LoadComponentCamera(GameObject* owner, json camerajsonRoot)
{
	glm::dvec3 vec0 = glm::dvec3(0);

	double fov, ratio, clipnear, clipfar, camoffset;

	fov = camerajsonRoot["Fov"];
	ratio = camerajsonRoot["Ratio"];
	clipnear = camerajsonRoot["Clipping Plane View Near"];
	clipfar = camerajsonRoot["Clipping Plane View Far"];
	camoffset = camerajsonRoot["Camera Offset"];

	Camera newcamera(owner, fov, ratio, clipnear, clipfar, camoffset, vec0);
	owner->AddComponent<Camera>(newcamera);
}