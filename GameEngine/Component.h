#pragma once
#include <memory>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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

	unsigned long UUID;

public:

	Component(GameObject* gameObject) : owner(gameObject) {}
	virtual ~Component() = default;

	virtual void Update() = 0;
	//virtual void Render() = 0;
	virtual Type getType() const = 0;

	virtual json SaveInfo() = 0;
};