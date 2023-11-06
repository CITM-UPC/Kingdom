#include "GameObject.h"
#include <memory>

GameObject::GameObject()
{
	name = "";
	components.push_back(std::make_shared<Transform>());
}

GameObject::~GameObject() = default;

std::shared_ptr<Component> GameObject::GetComponent(Component::Type componentType)
{
	for (auto& comp : components)
	{
		if (comp->getType() == componentType)
		{
			return comp;
		}
	}

	return nullptr;
}

std::vector<std::shared_ptr<Component>> GameObject::GetComponents()
{
	return components;
}

void GameObject::AddComponent(Component::Type component)
{
	std::shared_ptr<Component> ptr;

	switch (component)
	{
	case Component::Type::TRANSFORM:
		ptr = std::make_shared<Transform>();
		break;
	case Component::Type::MESH:
		ptr = std::make_shared<Mesh>();
		break;
	case Component::Type::TEXTURE2D:
		ptr = std::make_shared<Texture2D>();
		break;
	default:
		ENGINE_LOG("Cant add component to components list in GameObject");
		break;
	}

	ptr->gameObject = this;
	components.push_back(ptr);
}

void GameObject::AddComponent(std::shared_ptr<Mesh> component)
{
	component->gameObject = this;
	components.push_back(component);
}

void GameObject::AddComponent(std::shared_ptr<Texture2D> component)
{
	component->gameObject = this;
	components.push_back(component);
}

void GameObject::RemoveComponent(Component::Type component)
{
	for (auto& comp : components)
	{
		if (comp->getType() == component)
		{
			components.erase(std::remove(components.begin(), components.end(), comp));
			break;
		}
	}
}

GameObject* GameObject::Find(std::string name, std::list<GameObject> gameObjectList)
{
	for (auto& go : gameObjectList)
	{
		if (go.name == name)
		{
			return &go;
		}
	}
	return nullptr;
}

void GameObject::UpdateComponents()
{
	for (auto& comp : components)
	{
		comp->Update();
	}
}