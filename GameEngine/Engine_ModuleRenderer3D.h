#pragma once
#include "Engine_Module.h"
#include "Engine_Globals.h"
#include "SDL2/SDL.h"
#include <glm/gtc/type_ptr.hpp>

class Engine_ModuleRenderer3D : public Engine_Module
{
public:
	Engine_ModuleRenderer3D(GameEngine* gEngine, bool start_enabled = true);
	~Engine_ModuleRenderer3D();

	bool Init();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

	void SetTargetWindow(SDL_Window* target) { 
		targetWindow = target; 
	}
	void SetScreenSize(int width, int height) { 
		screen_width = width; 
		screen_height = height;
	}
	void SetVsync(bool active) { 
		vsync = active; 
	}

public:

	SDL_GLContext context;
	glm::mat3x3 NormalMatrix;
	glm::mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:

	SDL_Window* targetWindow;
	bool vsync;
	int screen_width;
	int screen_height;
};