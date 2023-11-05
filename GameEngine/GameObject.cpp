#include "GameObject.h"
#include <memory>

GameObject::GameObject()
{
	AddComponent(Component::Type::TRANSFORM);
	//components.push_back(std::make_unique<Transform>().release());
}

GameObject::~GameObject() = default;

Component* GameObject::GetComponent(Component::Type componentType)
{
	for (auto const& comp : components)
	{
		if (comp->type == componentType)
		{
			return comp;
		}
	}

	return nullptr;
}

void GameObject::AddComponent(Component::Type component)
{
	std::unique_ptr<Component> ptr;
	
	switch (component)
	{
	case Component::Type::TRANSFORM:
		ptr = std::make_unique<Transform>();
		break;
	case Component::Type::MESH:
		ptr = std::make_unique<Mesh>();
		break;
	case Component::Type::TEXTURE2D:
		ptr = std::make_unique<Texture2D>();
		break;
	default:
		ENGINE_LOG("Cant add component to components list in GameObject");
		break;
	}

	components.push_back(ptr.release());
}

void GameObject::AddComponent(Mesh component)
{
	std::unique_ptr<Component> ptr = std::make_unique<Mesh>(component);

	components.push_back(ptr.release());
}

void GameObject::RemoveComponent(Component::Type component)
{
	for (auto& comp : components)
	{
		if (comp->type == component)
		{
			components.erase(std::remove(components.begin(),components.end(), comp));
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
