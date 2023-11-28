#include "GameObject.h"
#include <memory>

GameObject::GameObject()
{
	name = "";
	components.push_back(std::move(std::make_unique<Transform>(this)));
}

GameObject::~GameObject() = default;

//std::shared_ptr<Component> GameObject::GetComponent(Component::Type componentType)
//{
//	for (auto& comp : components)
//	{
//		if (comp->getType() == componentType)
//		{
//			return comp;
//		}
//	}
//
//	return nullptr;
//}

std::vector<std::unique_ptr<Component>>* GameObject::GetComponents()
{
	return &components;
}

void GameObject::AddComponent(Component::Type component)
{
	std::unique_ptr<Component> compToAdd;

	switch (component)
	{
	case Component::Type::TRANSFORM:
		compToAdd = std::make_unique<Transform>(this);
		break;
	case Component::Type::MESH:
		compToAdd = std::make_unique<Mesh>(this);
		break;
	case Component::Type::TEXTURE2D:
		compToAdd = std::make_unique<Texture2D>(this);
		break;
	case Component::Type::CAMERA:
		compToAdd = std::make_unique<Camera>(this);
		break;
	default:
		ENGINE_LOG("Can't add component in GameObject");
		break;
	}

	components.push_back(std::move(compToAdd));
}

void GameObject::AddComponent(std::unique_ptr<Component> component)
{
	components.push_back(std::move(component));
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

//GameObject* GameObject::Find(std::string name, std::list<GameObject> gameObjectList)
//{
//	for (auto& go : gameObjectList)
//	{
//		if (go.name == name)
//		{
//			return &go;
//		}
//	}
//	return nullptr;
//}

void GameObject::UpdateComponents()
{
	for (auto& comp : components)
	{
		comp->Update();
	}
}