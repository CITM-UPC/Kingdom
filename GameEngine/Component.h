#pragma once
#include <memory>
#include "GameObject.h"

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