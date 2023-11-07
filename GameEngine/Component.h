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
		TEXTURE2D,
		CAMERA
	};

	bool isActive = true;
	GameObject& gameObject;

public:

	Component(GameObject& owner) : gameObject(owner) {}
	virtual void Update() = 0;
	virtual Type getType() const = 0;
};