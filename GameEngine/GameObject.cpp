#include "GameObject.h"
#include <memory>
#include <GL/glew.h>

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

		drawAABBox(computeAABB());
	}
}

AABBox GameObject::computeAABB()
{
	AABBox aabbox;

	Mesh* meshComponent = GetComponent<Mesh>();

	if (meshComponent != nullptr) aabbox = meshComponent->aabb;

	//To implement with tree structure
	/*
	else if (children().empty())
	{
		aabbox.min = vec3(0);
		aabbox.max = vec3(0);
	}

	for (const auto& child : children())
	{
		const auto child_aabb = (child.transform() * child.aabb()).AABB();
		aabbox.min = glm::min(aabbox.min, child_aabb.min);
		aabbox.max = glm::max(aabbox.max, child_aabb.max);
	}
	*/
	return aabbox;
}

static inline void glVec3(const vec3& v) { glVertex3dv(&v.x); }

static void drawAABBox(const AABBox& aabb) {
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);

	glVec3(aabb.a());
	glVec3(aabb.b());
	glVec3(aabb.c());
	glVec3(aabb.d());
	glVec3(aabb.a());

	glVec3(aabb.e());
	glVec3(aabb.f());
	glVec3(aabb.g());
	glVec3(aabb.h());
	glVec3(aabb.e());
	glEnd();

	glBegin(GL_LINES);
	glVec3(aabb.h());
	glVec3(aabb.d());
	glVec3(aabb.f());
	glVec3(aabb.b());
	glVec3(aabb.g());
	glVec3(aabb.c());
	glEnd();
}