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

bool ModuleUI::Init()
{
	LOG("Creating UI");


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context); //Creating jaja dependencies for testing purposes
	ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleUI::PreUpdate()
{
	//This here does not work. Currently in Input.cpp
	//ImGui_ImplSDL2_ProcessEvent(&pollevent); 

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	#pragma region	ImGui_MenuBar_Test

	if (dockSpaceEnabled)
		ImGui::DockSpaceOverViewport();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("Example1.example");
				ImGui::MenuItem("Example1.example");
				ImGui::MenuItem("Example1.example");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Example2.example");
					ImGui::MenuItem("Example2.example");
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			if (ImGui::MenuItem("Exit")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) 
			{
				LOG("Pressed Paste Button");
			}
			if (ImGui::MenuItem("Toggle DockSpace"))
			{
				dockSpaceEnabled = !dockSpaceEnabled;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	#pragma endregion

	#pragma region ImGui_Windows_Test

	ImGui::ShowDemoWindow(); // Show demo window! :)

	ImGui::Begin("Window A");
	ImGui::Text("This is window A");
	ImGui::Button("Button on window A");
	ImGui::End();

	ImGui::Begin("Window B");
	ImGui::Text("This is window B");
	ImGui::End();

	ImGui::Begin("Window B");
	ImGui::Button("Button on window B");
	ImGui::End();

	ImGui::Begin("Window C");
	ImGui::Text("This is window C");
	if (ImGui::Button("Button on window C")) { LOG("Button on window C pressed"); }
	if (ImGui::Button("Close editor")) { return UPDATE_STOP; }
	ImGui::End();

	#pragma endregion

	return UPDATE_CONTINUE;
}

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
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
