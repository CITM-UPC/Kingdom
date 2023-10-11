#pragma once

class GameEngine;

class Engine_Module
{
private :
	bool enabled;

public:
	GameEngine* gEngine;

	Engine_Module(GameEngine* parent, bool start_enabled = true) : gEngine(parent)
	{}

	virtual ~Engine_Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual engine_status PreUpdate()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual engine_status Update()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual engine_status PostUpdate()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}
};