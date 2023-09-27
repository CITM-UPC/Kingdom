#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"

#include "ModuleInput.h" //All of this is testing
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleUI::~ModuleUI()
{}

// Called before render is available
bool ModuleUI::Init()
{
	LOG("Creating UI");


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context); //Creating jaja dependencies for testing purposes
	ImGui_ImplOpenGL3_Init("#version 330");

	return true;
}

// PreUpdate: clear buffer
update_status ModuleUI::PreUpdate()
{
	//This here does not work. Currently in Input.cpp
	//ImGui_ImplSDL2_ProcessEvent(&pollevent); 

	// (After event loop)
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(); // Show demo window! :)

	#pragma region	ImGuiTest2

	/*ImGui::Begin("Window A");
	ImGui::Text("This is window A");
	ImGui::End();

	ImGui::Begin("Window B");
	ImGui::Text("This is window B");
	ImGui::End();

	ImGui::Begin("Window A");
	ImGui::Button("Button on window A");
	ImGui::End();

	ImGui::Begin("Window B");
	ImGui::Button("Button on window B");
	ImGui::End();*/

	#pragma endregion

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleUI::CleanUp()
{
	LOG("Destroying UI");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleUI::RenderUI()
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls SDL_GL_SwapWindow() etc.)
}
