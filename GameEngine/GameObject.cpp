#include "GameObject.h"
#include <memory>

GameObject::GameObject()
{
	name = "";
	Transform transformToPush(this);
	AddComponent(transformToPush);
}

GameObject::~GameObject()
{
	components.clear();
};

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

std::list<std::unique_ptr<Component>>* GameObject::GetComponents()
{
	return &components;
}

void GameObject::AddComponent(Component::Type component)
{
	switch (component)
	{
	case Component::Type::TRANSFORM:
		components.emplace_back(std::make_unique<Transform>(this));
		break;
	case Component::Type::MESH:
		components.emplace_back(std::make_unique<Mesh>(this));
		break;
	case Component::Type::TEXTURE2D:
		components.emplace_back(std::make_unique<Texture2D>(this));
		break;
	case Component::Type::CAMERA:
		components.emplace_back(std::make_unique<Camera>(this));
		break;
	default:
		ENGINE_LOG("Can't add component in GameObject");
		break;
	}
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

void GameObject::Move(GameObject* newParent)
{
	auto it = std::find_if(parent->childs.begin(), parent->childs.end(), [this](const std::unique_ptr<GameObject>& child) {
		return child.get() == this;
		});

	if (it != parent->childs.end()) {
		// Move the child to the new list
		newParent->childs.push_back(std::move(*it));
		parent->childs.erase(it);

		// Update the parent pointer of the moved child
		parent = newParent;
	}
}

void GameObject::removeChild(GameObject* child)
{
	for (auto& childit : childs)
	{
		if (childit.get() == child)
		{
			childit.release();
		}
	}
}