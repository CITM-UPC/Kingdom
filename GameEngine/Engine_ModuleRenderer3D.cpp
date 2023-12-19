#include "GameEngine.h"
#include "Engine_Globals.h"
#include "Engine_ModuleRenderer3D.h"
#include <GL/glew.h>
#include "SDL2/SDL_opengl.h"
#include <IL/il.h>
#include <iostream>

Engine_ModuleRenderer3D::Engine_ModuleRenderer3D(GameEngine* gEngine, bool start_enabled) : Engine_Module(gEngine, start_enabled)
{
	vsync = false;
	screen_width = 1024;
	screen_height = 768;
}

// Destructor
Engine_ModuleRenderer3D::~Engine_ModuleRenderer3D() = default;

// Called before render is available
bool Engine_ModuleRenderer3D::Init()
{
	logHistory.push_back("[Engine] Creating 3D Renderer context");
	ENGINE_LOG("Creating 3D Renderer context");
	bool ret = true;

	if (targetWindow == nullptr)
	{
		ENGINE_LOG("Target window has not been set. Try initializing the variable with 'SetTargetWindow()'");
		ret = false;
	}

	//Create context
	context = SDL_GL_CreateContext(targetWindow);
	if (context == nullptr)
	{
		ENGINE_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		// Initialize DevIL
		logHistory.push_back("[Engine] Initializing DevIL");
		ilInit();

		// Initialize glew
		logHistory.push_back("[Engine] Initializing OpenGL");
		glewInit();

		//Use Vsync
		if (vsync && SDL_GL_SetSwapInterval(1) < 0)
			ENGINE_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			ENGINE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			ENGINE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			ENGINE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		depth_test = true;

		glEnable(GL_CULL_FACE);
		cull_face = true;

		glEnable(GL_COLOR_MATERIAL);
		color_material = true;

		//glEnable(GL_LIGHTING);
		lighting = false;

		line_smooth = false;

		polygon_smooth = false;
	}

	// Projection matrix for
	OnResize(screen_width, screen_height);

	addGameObject("Assets/BakerHouse.fbx");
	gameObjectList.back()->GetComponent<Transform>()->Move(vec3(-2, 0, 0), Transform::Space::GLOBAL);

	return ret;
}

// PreUpdate: clear buffer
engine_status Engine_ModuleRenderer3D::PreUpdate()
{
	gEngine->cameraGO.UpdateComponents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(gEngine->cameraGO.GetComponent<Camera>()->fov,
		gEngine->cameraGO.GetComponent<Camera>()->aspectRatio,
		gEngine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear,
		gEngine->cameraGO.GetComponent<Camera>()->clippingPlaneViewFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gEngine->cameraGO.GetComponent<Camera>()->lookAtPos = gEngine->cameraGO.GetComponent<Transform>()->position() + gEngine->cameraGO.GetComponent<Transform>()->forward() * gEngine->cameraGO.GetComponent<Camera>()->camOffset;

	gluLookAt(gEngine->cameraGO.GetComponent<Transform>()->position().x, gEngine->cameraGO.GetComponent<Transform>()->position().y, gEngine->cameraGO.GetComponent<Transform>()->position().z,
		gEngine->cameraGO.GetComponent<Camera>()->lookAtPos.x, gEngine->cameraGO.GetComponent<Camera>()->lookAtPos.y, gEngine->cameraGO.GetComponent<Camera>()->lookAtPos.z,
		gEngine->cameraGO.GetComponent<Transform>()->up().x, gEngine->cameraGO.GetComponent<Transform>()->up().y, gEngine->cameraGO.GetComponent<Transform>()->up().z);

	return ENGINE_UPDATE_CONTINUE;
}

engine_status Engine_ModuleRenderer3D::Update()
{
	//Temporary, only debug
	glLineWidth(3.0);
	glBegin(GL_LINES);

	double size = 0.05;
	for (int i = 0; i < origins.size(); i++)
	{
		glColor3ub(0, 255, 0);
		glVertex3d(camPos[i].x, camPos[i].y, camPos[i].z - size);
		glVertex3d(camPos[i].x, camPos[i].y, camPos[i].z + size);
		glVertex3d(camPos[i].x, camPos[i].y - size, camPos[i].z);
		glVertex3d(camPos[i].x, camPos[i].y + size, camPos[i].z);
		glVertex3d(camPos[i].x - size, camPos[i].y, camPos[i].z);
		glVertex3d(camPos[i].x + size, camPos[i].y, camPos[i].z);

		glColor3ub(255, 255, 0);
		glVertex3d(origins[i].x, origins[i].y, origins[i].z);
		glColor3ub(255, 255, 255);
		glVertex3d(ends[i].x, ends[i].y, ends[i].z);
	}

	for (int i = 0; i < nearPlanes.size() / 4; i++)
	{
		glColor3ub(50, 150, 255);
		glVertex3d(nearPlanes[i].x, nearPlanes[i].y, nearPlanes[i].z);
		glVertex3d(nearPlanes[i + 1].x, nearPlanes[i + 1].y, nearPlanes[i + 1].z);

		glVertex3d(nearPlanes[i + 1].x, nearPlanes[i + 1].y, nearPlanes[i + 1].z);
		glVertex3d(nearPlanes[i + 2].x, nearPlanes[i + 2].y, nearPlanes[i + 2].z);

		glVertex3d(nearPlanes[i + 2].x, nearPlanes[i + 2].y, nearPlanes[i + 2].z);
		glVertex3d(nearPlanes[i + 3].x, nearPlanes[i + 3].y, nearPlanes[i + 3].z);

		glVertex3d(nearPlanes[i + 3].x, nearPlanes[i + 3].y, nearPlanes[i + 3].z);
		glVertex3d(nearPlanes[i].x, nearPlanes[i].y, nearPlanes[i].z);
	}

	glEnd();

	return ENGINE_UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
engine_status Engine_ModuleRenderer3D::PostUpdate()
{
	for (auto& vector : gameObjectList) {
		vector->UpdateComponents();
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}
	assert(error == GL_NO_ERROR);

	return ENGINE_UPDATE_CONTINUE;
}

// Called before quitting
bool Engine_ModuleRenderer3D::CleanUp()
{
	//ENGINE_LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);
	targetWindow = nullptr;
	delete targetWindow;

	return true;
}

void Engine_ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gEngine->cam.aspectRatio = (float)width / (float)height;
	ProjectionMatrix = glm::perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(glm::value_ptr(ProjectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Engine_ModuleRenderer3D::DrawGrid(int size, int step, bool xzAxis, bool xyAxis, bool zyAxis)
{
	glLineWidth(1.0);
	glColor3ub(128, 128, 128);

	glBegin(GL_LINES);
	for (int i = -size; i <= size; i += step) {
		if (xzAxis)
		{
			//XZ plane
			glVertex3i(i, 0, -size);
			glVertex3i(i, 0, size);
			glVertex3i(-size, 0, i);
			glVertex3i(size, 0, i);
		}
		if (xyAxis)
		{
			//XY plane
			glVertex2i(i, -size);
			glVertex2i(i, size);
			glVertex2i(-size, i);
			glVertex2i(size, i);
		}
		if (zyAxis)
		{
			//ZY plane
			glVertex3i(0, i, -size);
			glVertex3i(0, i, size);
			glVertex3i(0, -size, i);
			glVertex3i(0, size, i);
		}
	}
	glEnd();

	//drawCubeTest();

	drawAxis();
}

void Engine_ModuleRenderer3D::addGameObject()
{
	std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

	std::string meshName = "GameObject";
	int currentCopies = checkNameAvailability(meshName);
	if (currentCopies > 0) {
		meshName.append("(" + std::to_string(currentCopies) + ")");
	}
	gameObjectToAdd->name = meshName;

	gameObjectList.push_back(std::move(gameObjectToAdd));

	logHistory.push_back("[Engine] Add GameObject");
}

void Engine_ModuleRenderer3D::addGameObject(const std::string & filePath)
{
	logHistory.push_back("[Engine] Add GameObject with path " + filePath);

	auto meshInfo_vector = MeshLoader::loadMeshFromFile(filePath);
	auto texture_paths_vector = MeshLoader::loadTextureFromFile(filePath);

	int i = 0;
	for (const auto& meshInfo : meshInfo_vector)
	{
		std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

		std::string fileName = filePath;
		eraseBeforeDelimiter(fileName);

		std::string meshName = fileName;
		deleteSubstring(meshName, ".fbx");
		int currentCopies = checkNameAvailability(meshName);
		if (currentCopies > 0) {
			meshName.append("(" + std::to_string(currentCopies) + ")");
		}
		gameObjectToAdd->name = meshName;

		gameObjectList.push_back(std::move(gameObjectToAdd));

		Texture2D textureToPush(gameObjectList.back().get(), texture_paths_vector.at(i));
		gameObjectList.back().get()->AddComponent<Texture2D>(textureToPush);

		Mesh meshToPush(gameObjectList.back().get(), meshInfo, Mesh::Formats::F_V3T2);
		gameObjectList.back().get()->AddComponent<Mesh>(meshToPush);

		gameObjectList.back().get()->GetComponent<Mesh>()->setName(fileName);
		gameObjectList.back().get()->GetComponent<Mesh>()->texture = gameObjectList.back().get()->GetComponent<Texture2D>();
		i++;

		logHistory.push_back("[Engine] Mesh loaded with " + std::to_string(meshInfo._numFaces) + " faces, "
			+ std::to_string(meshInfo._numIndexs) + " indexs, "
			+ std::to_string(meshInfo._numNormals) + " normals, "
			+ std::to_string(meshInfo._numTexCoords) + " tex coords, and "
			+ std::to_string(meshInfo._numVerts) + " vertexs.");
	}
}

void Engine_ModuleRenderer3D::addGameObject(Primitive* shape)
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

	logHistory.push_back("[Engine] Mesh (" + meshName + ") loaded with " + std::to_string(meshInfo._numFaces) + " faces, "
		+ std::to_string(meshInfo._numIndexs) + " indexs, "
		+ std::to_string(meshInfo._numNormals) + " normals, "
		+ std::to_string(meshInfo._numTexCoords) + " tex coords, and "
		+ std::to_string(meshInfo._numVerts) + " vertexs.");
}

void Engine_ModuleRenderer3D::SwapDepthTest()
{
	if (depth_test) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

void Engine_ModuleRenderer3D::SwapCullFace()
{
	if (cull_face) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void Engine_ModuleRenderer3D::SwapLighting()
{
	if (lighting) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}
}

void Engine_ModuleRenderer3D::SwapColorMaterial()
{
	if (color_material) {
		glEnable(GL_COLOR_MATERIAL);
	}
	else {
		glDisable(GL_COLOR_MATERIAL);
	}
}

void Engine_ModuleRenderer3D::SwapLineSmooth()
{
	if (line_smooth) {
		glEnable(GL_LINE_SMOOTH);
	}
	else {
		glDisable(GL_LINE_SMOOTH);
	}
}

void Engine_ModuleRenderer3D::SwapPolygonSmooth()
{
	if (polygon_smooth) {
		glEnable(GL_POLYGON_SMOOTH);
	}
	else {
		glDisable(GL_POLYGON_SMOOTH);
	}
}