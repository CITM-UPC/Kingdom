#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Component.h"

class GameObject
{
public:

	std::string name;
	bool isActive;

private:

	std::vector<std::unique_ptr<Component>> components;

public:

	GameObject();
	~GameObject();

	Component* GetComponent(Component::Type componentType);
	void AddComponent(Component::Type component);
	void RemoveComponent(Component::Type component);

	static GameObject* Find(std::string name, std::list<GameObject> gameObjectList);

private:

	void UpdateComponents();
};

