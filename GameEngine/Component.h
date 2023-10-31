#pragma once
#include <memory>
#include "GameObject.h"

class GameObject;

class Component
{
public:

	enum Type
	{
		TRANSFORM,
		MESH,
		TEXTURE2D,
	};

	Type type;
	bool isActive;
	std::shared_ptr<GameObject> gameObject;

public:

	virtual void Update() = 0;
	virtual void DrawInspector() = 0;
};