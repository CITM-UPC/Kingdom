#include "GameObject.h"
#include <memory>

GameObject::GameObject()
{
	components.push_back(std::make_shared<Transform>());
	//AddComponent(Component::Type::TRANSFORM);
	//components.push_back(std::make_unique<Transform>().release());
}

GameObject::~GameObject() = default;

std::vector<std::shared_ptr<Component>> GameObject::GetComponents(Component::Type componentType)
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
		ptr = std::make_unique<Mesh>();
		break;
	case Component::Type::TEXTURE2D:
		ptr = std::make_unique<Texture2D>();
		break;
	default:
		ENGINE_LOG("Cant add component to components list in GameObject");
		break;
	}

	components.push_back(ptr);
}

void GameObject::AddComponent(std::shared_ptr<Mesh> component)
{
	components.push_back(component);
}

void GameObject::RemoveComponent(Component::Type component)
{
	for (auto& comp : components)
	{
		if (comp->getType() == component)
		{
			components.erase(std::remove(components.begin(),components.end(), comp));
			break;
		}
	}
}
//void GameObject::RemoveComponent(Component::Type component)
//{
//	components.erase(std::remove_if(components.begin(), components.end(),
//		[component](const std::unique_ptr<Component>& comp) {
//			return comp->type == component;
//		}), components.end());
//}

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
