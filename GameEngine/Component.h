#pragma once
#include <memory>

class GameObject;

class Component
{
public:

	enum class Type
	{
		TRANSFORM,
		MESH,
		TEXTURE2D
	};

	Type type;
	bool isActive = true;
	std::shared_ptr<GameObject> gameObject;

public:

	virtual void Update() = 0;
};