#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"

#include "ModuleInput.h" //All of this is testing
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleUI::~ModuleUI()
{}

void OsOpenInShell(const char* path)
{
#ifdef _WIN32
	// Note: executable path must use backslashes!
	::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
	const char* open_executable = "open";
#else
	const char* open_executable = "xdg-open";
#endif
	char command[256];
	snprintf(command, 256, "%s \"%s\"", open_executable, path);
	system(command);
#endif
}

bool ModuleUI::Init()
{
	LOG("Creating UI");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.Fonts->AddFontFromFileTTF("Fonts/DroidSans.ttf", 14);

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->gEngine->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	std::ifstream file("../LICENSE"); // Open the file
	std::string line;
	if (file.is_open()) { // Check if the file is open
		while (std::getline(file, line)) { // Read the file line by line
			aboutContent += line + "\n"; // Append each line to the string
		}
		file.close(); // Close the file
	}
	else {
		LOG("Unable to open LICENSE file.");
	}

	return true;
}

update_status ModuleUI::PreUpdate()
{
	//This here does not work. Currently in Input.cpp
	//ImGui_ImplSDL2_ProcessEvent(&pollevent);
	GetInfrastructureInfo();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (dockSpaceEnabled)
	{
		ImGuiDockNodeFlags dock_flags = 0;
		dock_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpaceOverViewport(0, dock_flags);
	}

	if (FPSgraph)   FPSGraphWindow();
	if (logWindow)	LogConsoleTestWindow();

	if (options)	OptionsWindow();
	if (camDebug)	CamDebugWindow();
	if (about)      AboutWindow();
	if (inspector)	InspectorWindow();
	if (hierarchy)	HierarchyWindow();
	if (infrastructure)   HardwareWindow();

#pragma region ImGui_Windows_Test

	/*ImGui::Begin("Inspector");
	ImGui::Text("First inspector test");
	ImGui::SliderFloat3("Position", (float*)&App->gEngine->cam.transform.position, 0, 1);
	ImGui::DragFloat("x", (float*)&App->gEngine->cam.transform.position.x);
	ImGui::SameLine();
	ImGui::DragFloat("y", (float*)&App->gEngine->cam.transform.position.y);
	ImGui::SameLine();
	ImGui::DragFloat("z", (float*)&App->gEngine->cam.transform.position.z);
	ImGui::End();*/

#pragma endregion3

	return MainMenuBar();
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

update_status ModuleUI::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Not implemented")) {}
			if (ImGui::MenuItem("Open Scene", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Not implemented")) {}
			if (ImGui::MenuItem("Save As...", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("New Project", "Not implemented")) {}
			if (ImGui::MenuItem("Open Project", "Not implemented")) {}
			if (ImGui::MenuItem("Save Project", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
			if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Select All", "Not implemented")) {}
			if (ImGui::MenuItem("Deselect All", "Not implemented")) {}
			if (ImGui::MenuItem("Select Children", "Not implemented")) {}
			if (ImGui::MenuItem("Invert Children", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Duplicate", "Not implemented")) {}
			if (ImGui::MenuItem("Delete", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Play", "Not implemented")) {}
			if (ImGui::MenuItem("Pause", "Not implemented")) {}
			if (ImGui::MenuItem("Step", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Settings", "Not implemented")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Config Window")) options = true;
			if (ImGui::MenuItem("Camera Debug")) camDebug = true;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::BeginMenu("Menus")) {
				ImGui::MenuItem("Hierarchy", "", &hierarchy);
				ImGui::MenuItem("Inspector", "", &inspector);
				ImGui::MenuItem("Log", "", &logWindow);
				ImGui::MenuItem("FPS Graph", "", &FPSgraph);
				ImGui::MenuItem("Infrastructure Information", "", &infrastructure);
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Toggle DockSpace", "", &dockSpaceEnabled)) dockSpaceEnabled = !dockSpaceEnabled;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObjects")) {
			if (ImGui::BeginMenu("Create...")) {
				if (ImGui::MenuItem("Create Empty")) { App->gEngine->renderer3D->addGameObject(); }
				if (ImGui::MenuItem("Plane", "Not implemented")) {}
				if (ImGui::MenuItem("Cube", "Not implemented")) {}
				if (ImGui::MenuItem("Sphere", "Not implemented")) {}
				if (ImGui::MenuItem("Cylinder", "Not implemented")) {}
				if (ImGui::MenuItem("Cone", "Not implemented")) {}
				if (ImGui::MenuItem("Torus", "Not implemented")) {}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Draw Mode")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) about = true;
			ImGui::Separator();
			if (ImGui::MenuItem("Check releases...")) { OsOpenInShell("https://github.com/CITM-UPC/Kingdom/releases"); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	return UPDATE_CONTINUE;
}

void ModuleUI::FPSGraphWindow()
{
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("FPS Graph", &FPSgraph, flags);

	std::stringstream sStream;
	sStream << "Average FPS: " << App->fpsHistory[App->fpsHistory.size() - 1];
	string title = sStream.str();

	ImGui::Text("FPS lines graph");
	ImGui::PlotLines("", &App->fpsHistory[0], App->fpsHistory.size(), 0, title.c_str(), 1.0f, 100.0f, { 325, 100 });
	ImGui::Separator();
	ImGui::Text("FPS histogram");
	ImGui::PlotHistogram("", &App->fpsHistory[0], App->fpsHistory.size(), 0, title.c_str(), 1.0f, 100.0f, { 325, 100 });
	ImGui::End();
}

void ModuleUI::HierarchyWindow()
{
	ImGui::Begin("Hierarchy", &hierarchy);
	for (auto& gameObject : App->gEngine->renderer3D->gameObjectList) {

		if (ImGui::MenuItem(gameObject.name.c_str())) {
			gameObjSelected = gameObject;
		}
	}
	ImGui::EndMenu();


}

void ModuleUI::InspectorWindow()
{
	ImGui::Begin("Inspector", &inspector);
	if (gameObjSelected.name != "") {
		ImGui::Checkbox("Active", &gameObjSelected.isActive);
		ImGui::SameLine(); ImGui::Text("GameObject name: ");
		ImGui::SameLine(); ImGui::Text(gameObjSelected.name.c_str());
		for (auto& component : gameObjSelected.GetComponents()) {
			if (component.get()->getType() == Component::Type::TRANSFORM) {
				Transform* transform = dynamic_cast<Transform*>(component.get());
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
				{
					if (ImGui::BeginTable("", 4))
					{
						//ImGui::DragFloat("", &transform->sc.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Checkbox("Active", &transform->isActive);
						ImGui::Text("Position");
						ImGui::Text("Rotation");
						ImGui::Text("Scale");

						ImGui::TableSetColumnIndex(1);
						ImGui::Text("X");
						ImGui::Text(std::to_string(transform->position.x).c_str());
						ImGui::Text(std::to_string(transform->rotation.x).c_str());
						ImGui::Text("1");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text("Y");
						ImGui::Text(std::to_string(transform->position.y).c_str());
						ImGui::Text(std::to_string(transform->rotation.y).c_str());
						ImGui::Text("1");

						ImGui::TableSetColumnIndex(3);
						ImGui::Text("Z");
						ImGui::Text(std::to_string(transform->position.z).c_str());
						ImGui::Text(std::to_string(transform->rotation.z).c_str());
						ImGui::Text("1");

						ImGui::EndTable();
					}
				}
			}
			if (component.get()->getType() == Component::Type::MESH) {
				Mesh* mesh = dynamic_cast<Mesh*>(component.get());
				if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_None))
				{
					ImGui::Checkbox("Active", &mesh->isActive);
					ImGui::SameLine();  ImGui::Text("Filename: ");
					ImGui::SameLine();  ImGui::Text(mesh->getName().c_str());
					ImGui::Separator();
					ImGui::Text("Indexes: ");
					ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumIndexs()).c_str());
					/*ImGui::Text("Normals: ");
					ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumNormals()).c_str());*/
					ImGui::Text("Vertexs: ");
					ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumVerts()).c_str());
					/*ImGui::Text("Faces: ");
					ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumFaces()).c_str());*/
					ImGui::Separator();
				}
			}
			if (component.get()->getType() == Component::Type::TEXTURE2D) {
				Texture2D* texture2D = dynamic_cast<Texture2D*>(component.get());
				if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_None))
				{
				}
			}

		}

	}
	ImGui::EndMenu();
}

void ModuleUI::LogConsoleTestWindow()
{
	ImGui::Begin("Log Console", &logWindow);
	if (ImGui::Button("Clear")) { App->logHistory.clear(); }
	ImGui::SameLine(); 
	bool copy = ImGui::Button("Copy");
	ImGui::Separator();
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &autoScrollLog);
		ImGui::EndPopup();
	}
	if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
	ImGui::Separator();
	
	if (ImGui::BeginChild("", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
		if (copy) ImGui::LogToClipboard();

		for (const auto& log : App->logHistory)
			ImGui::Text(log.c_str());
		if (autoScrollLog && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
		ImGui::GetClipboardText();
	}
	ImGui::EndChild();
	ImGui::End();
}

void ModuleUI::OptionsWindow()
{
	ImGui::Begin("Options Window", &options);
	ImGui::Text("This window is a placeholder.\nFunctionality is WIP");
	static bool testBool = false;
	if (ImGui::Checkbox("VSYNC", &testBool)) { LOG("Checkbox Pressed"); };
	ImGui::End();
}

void ModuleUI::CamDebugWindow()
{
	ImGui::Begin("Cam Debug", &camDebug);
	ImGui::Text("Camera Position x: %f", App->gEngine->cam.transform.position.x);
	ImGui::Text("Camera Position y: %f", App->gEngine->cam.transform.position.y);
	ImGui::Text("Camera Position z: %f", App->gEngine->cam.transform.position.z);
	ImGui::Separator();
	ImGui::Text("LookAt Pos x: %f", App->gEngine->cam.lookAtPos.x);
	ImGui::Text("LookAt Pos y: %f", App->gEngine->cam.lookAtPos.y);
	ImGui::Text("LookAt Pos z: %f", App->gEngine->cam.lookAtPos.z);
	ImGui::Separator();
	ImGui::Text("Forward: %f, %f, %f", App->gEngine->cam.transform.forward.x, App->gEngine->cam.transform.forward.y, App->gEngine->cam.transform.forward.z);
	ImGui::Text("Right: %f, %f, %f", App->gEngine->cam.transform.right.x, App->gEngine->cam.transform.right.y, App->gEngine->cam.transform.right.z);
	ImGui::Text("Up: %f, %f, %f", App->gEngine->cam.transform.up.x, App->gEngine->cam.transform.up.y, App->gEngine->cam.transform.up.z);
	ImGui::Separator();
	ImGui::Text("RotMat: %f, %f, %f", App->gEngine->cam.transform.referenceFrameMat[0][0], App->gEngine->cam.transform.referenceFrameMat[0][1], App->gEngine->cam.transform.referenceFrameMat[0][2]);
	ImGui::Text("RotMat: %f, %f, %f", App->gEngine->cam.transform.referenceFrameMat[1][0], App->gEngine->cam.transform.referenceFrameMat[1][1], App->gEngine->cam.transform.referenceFrameMat[1][2]);
	ImGui::Text("RotMat: %f, %f, %f", App->gEngine->cam.transform.referenceFrameMat[2][0], App->gEngine->cam.transform.referenceFrameMat[2][1], App->gEngine->cam.transform.referenceFrameMat[2][2]);
	ImGui::End();
}

void ModuleUI::AboutWindow()
{
	ImGui::Begin("About...", &about, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Kingdom v0.5\nA 3D Game Engine for the Game Engines subject.\nBy Jonathan Cacay & Ethan Martin.");
	if (ImGui::Button("Repository Link")) { OsOpenInShell("https://github.com/CITM-UPC/Kingdom/"); }
	ImGui::Separator();
	ImGui::Text("3rd Party Libraries used :");
	ImGui::Bullet(); if (ImGui::Button("Assimp 5.2.5")) { OsOpenInShell("https://assimp-docs.readthedocs.io/"); }
	ImGui::Bullet(); if (ImGui::Button("DevIL 1.8.0#11")) { OsOpenInShell("https://openil.sourceforge.net/"); }
	ImGui::Bullet(); if (ImGui::Button("GLEW 2.2.0#3")) { OsOpenInShell("https://glew.sourceforge.net/"); }
	ImGui::Bullet(); if (ImGui::Button("GLM 2023-06-08")) { OsOpenInShell("https://glm.g-truc.net/0.9.5/index.html"); }
	ImGui::Bullet(); if (ImGui::Button("ImGUI 1.89.9")) { OsOpenInShell("https://imgui-test.readthedocs.io/"); }
	ImGui::Bullet(); if (ImGui::Button("jsoncpp 1.9.5")) { OsOpenInShell("https://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html"); }
	ImGui::Bullet(); if (ImGui::Button("OpenGL 2022-12-04#3")) { OsOpenInShell("https://www.opengl.org/"); }
	ImGui::Bullet(); if (ImGui::Button("SDL2 2.28.3")) { OsOpenInShell("https://wiki.libsdl.org/"); }
	ImGui::Separator();
	if (ImGui::CollapsingHeader("License")) { ImGui::Text(aboutContent.c_str()); }
	ImGui::End();
}

void ModuleUI::HardwareWindow()
{
	ImGui::Begin("Infrastructure information", &infrastructure, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Software Information:");
	ImGui::Text("SDL:");
	ImGui::Bullet(); ImGui::Text("Compiled: ");
	ImGui::SameLine(); ImGui::TextColored({ 0.352f, 0.386f, 0.750f, 1.0f }, info.sdl_version_compiled.c_str());
	ImGui::Bullet(); ImGui::Text("Linked: ");
	ImGui::SameLine(); ImGui::TextColored({ 0.352f, 0.386f, 0.750f, 1.0f }, info.sdl_version_linked.c_str());
	ImGui::Text("OpenGL:");
	ImGui::Bullet(); ImGui::TextColored({ 0.719f, 0.735f, 0.910f, 1.0f }, info.gl_version.c_str());
	ImGui::Text("DevIL:");
	ImGui::Bullet(); ImGui::TextColored({ 0.610f, 0.0488f, 0.142f, 1.0f }, info.devil_version.c_str());
	ImGui::Separator();

	ImGui::Text("GPU Information:");
	std::string textToShow = "GPU: " + info.Gpu;
	ImGui::Bullet(); ImGui::TextColored({ 0.2f, 1.0f, 0.0f, 1.0f }, textToShow.c_str());

	textToShow = "Vendor: " + info.GpuVendor;
	ImGui::Bullet(); ImGui::TextColored({ 0.2f, 1.0f, 0.0f, 1.0f }, textToShow.c_str());

	textToShow = "Driver: " + info.GpuDriver;
	ImGui::Bullet(); ImGui::TextColored({ 0.2f, 1.0f, 0.0f, 1.0f }, textToShow.c_str());

	ImGui::Separator();

	ImGui::Text("VRAM Information:");
	textToShow = "Budget: " + std::to_string(info.vram_mb_budget) + " mb";
	ImGui::Bullet(); ImGui::TextColored({ 0.0504f, 0.720f, 0.642f, 1.0f }, textToShow.c_str());

	textToShow = "Usage: " + std::to_string(info.vram_mb_usage) + " mb";
	ImGui::Bullet(); ImGui::TextColored({ 0.0504f, 0.720f, 0.642f, 1.0f }, textToShow.c_str());

	textToShow = "Available: " + std::to_string(info.vram_mb_available) + " mb";
	ImGui::Bullet(); ImGui::TextColored({ 0.0504f, 0.720f, 0.642f, 1.0f }, textToShow.c_str());

	ImGui::Separator();

	ImGui::Text("CPU Information:");

	textToShow = "CPU Cores: " + std::to_string(info.cpu_count);
	ImGui::Bullet(); ImGui::TextColored({ 0.890f, 0.876f, 0.0356f, 1.0f }, textToShow.c_str());

	textToShow = "CPU cache line size: " + std::to_string(info.l1_cachekb);
	ImGui::Bullet(); ImGui::TextColored({ 0.890f, 0.876f, 0.0356f, 1.0f }, textToShow.c_str());

	ImGui::End();
}

void ModuleUI::GetInfrastructureInfo()
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	info.sdl_version_compiled = std::to_string(compiled.major) + "." + std::to_string(compiled.minor) + "." + std::to_string(compiled.patch);
	info.sdl_version_linked = std::to_string(linked.major) + "." + std::to_string(linked.minor) + "." + std::to_string(linked.patch);

	info.gl_version = App->gEngine->getOpenGLVersion();
	info.devil_version = App->gEngine->getDevILVersion();

	info.GpuVendor.assign((const char*)glGetString(GL_VENDOR));
	info.Gpu.assign((const char*)glGetString(GL_RENDERER));
	info.GpuDriver.assign((const char*)glGetString(GL_VERSION));

	GLint vmem_budget = 0;
	GLint vmem_available = 0;
	GLint vmem_usage = 0;

	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vmem_budget);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vmem_available);

	vmem_usage = vmem_budget - vmem_available;

	info.vram_mb_budget = float(vmem_budget) / 1024.0f;
	info.vram_mb_usage = float(vmem_usage) / 1024.f;
	info.vram_mb_available = float(vmem_available) / 1024.f;

	info.cpu_count = SDL_GetCPUCount();
	info.l1_cachekb = SDL_GetCPUCacheLineSize();
}