#include "GameEngine.h"

GameEngine::GameEngine()
{
	//input = new ModuleInput(this);
	camera = new ModuleCamera3D(this);
	renderer3D = new ModuleRenderer3D(this);
	//audio = new ModuleAudio(this, true);

	//AddModule(input);
	AddModule(camera);
	AddModule(renderer3D);
	//AddModule(audio);
}

GameEngine::~GameEngine()
{
	list_modules.clear();
}

bool GameEngine::Init()
{
	bool ret = true;

	for (auto const& item : list_modules)
	{
		ret = item->Init();
	}

	LOG("Game Engine Start --------------");
	for (auto const& item : list_modules)
	{
		ret = item->Start();
	}

	return ret;
}

void GameEngine::PrepareUpdate()
{
}

void GameEngine::FinishUpdate()
{
}

update_status GameEngine::Update()
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

bool GameEngine::CleanUp()
{
	bool ret = true;

	for (auto const& item : list_modules)
	{
		ret = item->CleanUp();
		if (ret != true) return ret;
	}

	return ret;
}

void GameEngine::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}