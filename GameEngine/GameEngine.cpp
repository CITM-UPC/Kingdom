#include "GameEngine.h"

GameEngine::GameEngine()
{
	//input = new ModuleInput(this);
	renderer3D = new Engine_ModuleRenderer3D(this);

	//AddModule(input);
	AddModule(renderer3D);
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

	return ret;
}

bool GameEngine::Start()
{
	bool ret = true;

	ENGINE_LOG("Game Engine Start --------------");
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

engine_status GameEngine::PreUpdate()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;

	for (auto const& item : list_modules)
	{
		ret = item->PreUpdate();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

	return ret;
}

engine_status GameEngine::Update()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;
	PrepareUpdate();

	for (auto const& item : list_modules)
	{
		ret = item->Update();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

	FinishUpdate();
	return ret;
}

engine_status GameEngine::PostUpdate()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;

	for (auto const& item : list_modules)
	{
		ret = item->PostUpdate();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

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

void GameEngine::AddModule(Engine_Module* mod)
{
	list_modules.push_back(mod);
}