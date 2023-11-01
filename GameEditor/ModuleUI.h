#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>

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
	
private:

	bool dockSpaceEnabled = true;

	bool hierarchy = true;
	bool inspector = true;
	bool logWindow = true;
	bool FPSgraph = false;
	bool options = false;
	bool camDebug = false;
	bool about = false;

	std::string aboutContent;
};