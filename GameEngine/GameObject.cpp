#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetName(std::string name)
{
}

std::string GameObject::GetName()
{
	return std::string();
}

void GameObject::SetActive(bool isActive)
{
}

Component* GameObject::GetComponent(Component::Type component)
{
	return nullptr;
}

void GameObject::AddComponent(Component::Type component)
{
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
