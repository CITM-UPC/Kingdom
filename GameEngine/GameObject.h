#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "Camera.h"

class GameObject
{
public:

	std::string name;
	bool isActive = true;

private:

	std::vector<std::unique_ptr<Component>> components;

public:

	GameObject();
	~GameObject();

	template <typename T> T* GetComponent();
	//std::shared_ptr<Component> GetComponent(Component::Type componentType);

	std::vector<std::unique_ptr<Component>>* GetComponents();

	void AddComponent(Component::Type component);
	void AddComponent(std::unique_ptr<Component> component);

	void RemoveComponent(Component::Type component);

	void UpdateComponents();

	//static GameObject* Find(std::string name, std::list<GameObject> gameObjectList);

};

template<typename T>
inline T* GameObject::GetComponent()
{
	for (auto& component : components) {
		T* returnComponent = dynamic_cast<T*>(component.get());
		if (returnComponent) {
			return returnComponent;
		}
	}
	return nullptr;
}
