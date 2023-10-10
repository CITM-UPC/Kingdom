#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL2/SDL.h"
#include <glm/gtc/type_ptr.hpp>

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(GameEngine* gEngine, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate();
	update_status PostUpdate();
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
		VSYNC = active; 
	}

public:

	SDL_GLContext context;
	glm::mat3x3 NormalMatrix;
	glm::mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:

	SDL_Window* targetWindow;
	bool VSYNC;
	int screen_width;
	int screen_height;
};