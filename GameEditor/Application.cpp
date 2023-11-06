#include "Application.h"

Application::Application()
{
	gEngine = new GameEngine();

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer = new ModuleRenderer(this);
	ui = new ModuleUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(renderer);
	AddModule(ui);
}

Application::~Application()
{
	list_modules.clear();	//If this does not yield the expected results,
							//try using list::erase(i) traversing the list in reverse order.
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	logHistory.push_back("Initializing modules --------------");
	for (auto const& item : list_modules)
	{
		item->Init();
	}

	// After all Init calls we call Start() in all modules
	logHistory.push_back("Application Start --------------");
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
	update_status ret = update_status::UPDATE_CONTINUE;

	const auto frameStart = std::chrono::steady_clock::now();

	PrepareUpdate();

	for (auto const& item : list_modules)
	{
		ret = item->PreUpdate();
		if (ret != update_status::UPDATE_CONTINUE) return ret;
	}

	for (auto const& item : list_modules)
	{
		ret = item->Update();
		if (ret != update_status::UPDATE_CONTINUE) return ret;
	}

	for (auto const& item : list_modules)
	{
		ret = item->PostUpdate();
		if (ret != update_status::UPDATE_CONTINUE) return ret;
	}

	FinishUpdate();

	const auto frameEnd = std::chrono::steady_clock::now();
	const auto frameDuration = frameEnd - frameStart;

	if (frameDuration < frameDurationTime)
		this_thread::sleep_for(frameDurationTime - frameDuration);

	const auto frameEndAfterSleep = std::chrono::steady_clock::now();
	const auto frameDurationAfterSleep = frameEndAfterSleep - frameStart;

	float lastFPS = 1.0f / (frameDurationAfterSleep.count() * 0.000000001f);

	fpsHistory.push_back(lastFPS);

	// Replace oldest data in the history
	if (fpsHistory.size() > 100) fpsHistory.erase(fpsHistory.begin());

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	//This seems to work, but I think it is doing it in the normal order, not reverse.
	for (auto const& item : list_modules)
	{
		ret = item->CleanUp();
		if (ret != true) return ret;
	}

	delete gEngine;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}