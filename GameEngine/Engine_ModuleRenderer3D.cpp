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
Engine_ModuleRenderer3D::~Engine_ModuleRenderer3D()
{}

// Called before render is available
bool Engine_ModuleRenderer3D::Init()
{
	ENGINE_LOG("Creating 3D Renderer context");
	bool ret = true;

	if (targetWindow == NULL)
	{
		ENGINE_LOG("Target window has not been set. Try initializing the variable with 'SetTargetWindow()'");
		ret = false;
	}

	//Create context
	context = SDL_GL_CreateContext(targetWindow);
	if (context == NULL)
	{
		ENGINE_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		// Initialize DevIL
		ilInit();

		// Initialize glew
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
		glEnable(GL_CULL_FACE);
		//glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(screen_width, screen_height);

	//addGameObject("Assets/BakerHouse.fbx");

	return ret;
}

// PreUpdate: clear buffer
engine_status Engine_ModuleRenderer3D::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(gEngine->cam.fov, gEngine->cam.aspectRatio, gEngine->cam.clippingPlaneViewNear, gEngine->cam.clippingPlaneViewFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(gEngine->cam.transform.position.x, gEngine->cam.transform.position.y, gEngine->cam.transform.position.z,
		gEngine->cam.lookAtPos.x, gEngine->cam.lookAtPos.y, gEngine->cam.lookAtPos.z,
		gEngine->cam.transform.up.x, gEngine->cam.transform.up.y, gEngine->cam.transform.up.z);


	return ENGINE_UPDATE_CONTINUE;
}

engine_status Engine_ModuleRenderer3D::Update()
{

	return ENGINE_UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
engine_status Engine_ModuleRenderer3D::PostUpdate()
{
#pragma region TriangleTest

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4ub(255, 0, 0, 255);
	glBegin(GL_TRIANGLES);
	glVertex3d(-0.25, 0, 0);
	glVertex3d(0.25, 0, 0);
	glVertex3d(0, 0.5, 0);
	glEnd();*/

#pragma endregion

	for (auto& vector : gameObjectList) {
		vector.UpdateComponents();
	}

	GLenum error = glGetError();
	assert(error == GL_NO_ERROR);

	return ENGINE_UPDATE_CONTINUE;
}

// Called before quitting
bool Engine_ModuleRenderer3D::CleanUp()
{
	ENGINE_LOG("Destroying 3D Renderer");

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
	gEngine->cam.aspectRatio = (float)width / (float)height;
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
