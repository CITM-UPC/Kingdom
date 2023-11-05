#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>

struct HardwareInformation {
	SDL_version version;
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
	void HardwareWindow();

	void GetHardwareInformation();

private:

	bool dockSpaceEnabled = true;

	bool hierarchy = true;
	bool inspector = true;
	bool logWindow = true;
	bool FPSgraph = false;
	bool options = false;
	bool camDebug = false;
	bool about = false;
	bool hardware = false;

	std::string aboutContent;

	// Hardware information
	HardwareInformation info;
};