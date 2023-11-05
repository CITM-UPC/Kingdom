#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"

class GameObject
{
public:

	std::string name;
	bool isActive = true;

private:

	std::vector<std::shared_ptr<Component>> components;

public:

	GameObject();
	~GameObject();

	std::vector<std::shared_ptr<Component>> GetComponents(Component::Type componentType);
	void AddComponent(Component::Type component);
	void AddComponent(std::shared_ptr<Mesh> component);
	void RemoveComponent(Component::Type component);

	static GameObject* Find(std::string name, std::list<GameObject> gameObjectList);

	void UpdateComponents();
};

