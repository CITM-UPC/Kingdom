#include "GameObject.h"

#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetName(std::string name)
{
	this->name = name;
}

std::string GameObject::GetName()
{
	return this->name;
}

void GameObject::SetActive(bool isActive)
{
	this->isActive = isActive;
}

Component* GameObject::GetComponent(Component::Type componentType)
{
	for (auto const& comp : components)
	{
		if (comp->type == componentType)
		{
			return comp.get();
		}
	}

	return nullptr;
}

void GameObject::AddComponent(Component::Type component)
{
	std::unique_ptr<Component> ptr = nullptr;

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

	components.push_back(std::move(ptr));
}

void GameObject::RemoveComponent(Component::Type component)
{
}

GameObject GameObject::Find(std::string name)
{
	return GameObject();
}

void GameObject::UpdateComponents()
{
}
