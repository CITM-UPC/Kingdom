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

	bool isActive = true;
	GameObject* gameObject;
	
public:

	virtual void Update() = 0;
	virtual Type getType() const = 0;

};