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
	GameObject* owner;

public:

	Component(GameObject* gameObject) : owner(gameObject) {}

	virtual void Update() = 0;
	//virtual void Render() = 0;
	virtual Type getType() const = 0;
};