#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	camera = new ModuleCamera3D(this);
	renderer3D = new ModuleRenderer3D(this);
	ui = new ModuleUI(this);
	//audio = new ModuleAudio(this, true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(camera);
	AddModule(renderer3D);
	AddModule(ui);
	//AddModule(audio);
}

Application::~Application()
{
	/*
	p2List_item<Module*>* item = list_modules.getLast();

	while (item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
	*/

	
	list_modules.clear();	//If this does not yield the expected results, 
							//try using list::erase(i) traversing the list in reverse order.
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (auto const& item:list_modules)
	{
		item->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (auto const& item : list_modules)
	{
		item->Start();
	}
	
	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (auto const& item : list_modules)
	{
		ret = item->PreUpdate();
		if (ret != UPDATE_CONTINUE) return ret;
	}

	for (auto const& item : list_modules)
	{
		ret = item->Update();
		if (ret != UPDATE_CONTINUE) return ret;
	}

	for (auto const& item : list_modules)
	{
		ret = item->PostUpdate();
		if (ret != UPDATE_CONTINUE) return ret;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	//Original version
	/*
	p2List_item<Module*>* item = list_modules.getLast();
	auto item = list_modules.back();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	*/

	//Attempt to translate to STL
	/*
	auto item = list_modules.back();

	while(item != NULL && ret == true)
	{
		ret = item->CleanUp();
		item = std::prev(item, 1);
	}
	*/


	//This seems to work, but I think it is doing it in the normal order, not reverse.
	for (auto const& item: list_modules)
	{
		ret = item->CleanUp();
		if (ret != true) return ret;
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}