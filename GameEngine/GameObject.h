#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Component.h"

class GameObject
{
private:

	std::string name;
	bool isActive;
	std::vector<std::unique_ptr<Component>> components;

public:

	GameObject();
	~GameObject();

	void SetName(std::string name);
	std::string GetName();

	void SetActive(bool isActive);

	Component* GetComponent(Component::Type componentType);
	void AddComponent(Component::Type component);
	void RemoveComponent(Component::Type component);

	static GameObject Find(std::string name);

private:

	void UpdateComponents();
};

