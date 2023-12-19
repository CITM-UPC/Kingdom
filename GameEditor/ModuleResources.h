#pragma once
#include "Module.h"
#include "Globals.h"
#include <map>

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	/*UID Find(const char* file_in_assets) const;
	UID ImportFile(const char* new_file_in_assets);
	UID GenerateNewUID();

	const Resource* RequestResource(UID uid) const;
	Resource* RequestResource(UID uid);
	void ReleaseResource(UID uid);*/

private:
	//Resource* CreateNewResource(const char* assetsFile, Resource::Type type);

private:
	//std::map<UID, Resource*> resources;
};
