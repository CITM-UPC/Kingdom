#pragma once
#include <memory>
#include "json/json.h"

class GameObject;

class Component
{
public:

	enum class Type
	{
		TRANSFORM,
		MESH,
		TEXTURE2D,
		CAMERA
	};

	bool isActive = true;
	GameObject* owner;

	unsigned long UID;

public:

	Component(GameObject* gameObject) : owner(gameObject) {}
	virtual ~Component() = default;

	virtual void Update() = 0;
	//virtual void Render() = 0;
	virtual Type getType() const = 0;

	virtual Json::Value SaveInfo() = 0;
};