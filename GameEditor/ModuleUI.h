#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>
#include <cstring>
#include "..\GameEngine\GameObject.h"
#include "..\GameEngine\Component.h"

struct InfrastructureInfo {
	std::string sdl_version_compiled;
	std::string sdl_version_linked;
	std::string gl_version;
	std::string devil_version;

	std::string GpuVendor;
	std::string Gpu;
	std::string GpuDriver;

	float vram_mb_budget = 0.f;
	float vram_mb_usage = 0.f;
	float vram_mb_available = 0.f;

	uint cpu_count = 0;
	uint l1_cachekb = 0;
};

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	void RenderUI();

private:

	update_status MainMenuBar();

	void FPSGraphWindow();
	void HierarchyWindow();
	void InspectorWindow();
	void LogConsoleTestWindow();
	void OptionsWindow();
	void CamDebugWindow();
	void AboutWindow();

	void GetInfrastructureInfo();

private:

	bool dockSpaceEnabled = true;

	bool hierarchy = true;
	bool inspector = true;
	bool logWindow = true;
	bool FPSgraph = false;
	bool options = false;
	bool camDebug = false;
	bool about = false;

	bool autoScrollLog = true;

	std::string aboutContent;

	// Hardware information
	InfrastructureInfo info;
	GameObject gameObjSelected;
};