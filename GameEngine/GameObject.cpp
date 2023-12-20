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

static inline void glVec3(const vec3& v) { glVertex3dv(&v.x); }

void GameObject::UpdateComponents()
{
	for (auto& comp : components)
	{
		comp->Update();
	}
}

void GameObject::RenderComponents()
{
	for (auto& comp : components)
	{
		glPushMatrix();
		glMultMatrixd(&GetComponent<Transform>()->_transformationMatrix[0].x);
		comp->Render();
		glPopMatrix();
	}
}

AABBox GameObject::computeAABB()
{
	AABBox aabbox;

	Mesh* meshComponent = GetComponent<Mesh>();
	if (meshComponent != nullptr)
	{
		aabbox = meshComponent->getAABB();
		const auto obBox = GetComponent<Transform>()->_transformationMatrix * aabbox;
		aabbox = obBox.AABB();
	}
	else
	{
		aabbox.min = vec3(0);
		aabbox.max = vec3(0);
	}

	for (const auto& child : childs)
	{
		const auto child_aabb = child->computeAABB();
		aabbox.min = glm::min(aabbox.min, child_aabb.min);
		aabbox.max = glm::max(aabbox.max, child_aabb.max);
	}

	if (parent == nullptr)
	{
		const auto obBox = GetComponent<Transform>()->_transformationMatrix * aabbox;
		aabbox = obBox.AABB();
	}

	return aabbox;
}

void GameObject::drawAABBox(const AABBox& aabb)
{
	glColor3ub(0, 200, 255);
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

void GameObject::Move(GameObject* newParent, std::list<unique_ptr<GameObject>>& listToCheck)
{
	std::_List_iterator it = std::find_if(listToCheck.begin(), listToCheck.end(), [this](const std::unique_ptr<GameObject>& child) {
		return child.get() == this;
		});

	if (it != listToCheck.end()) {
		// Move the child to the new list
		newParent->childs.push_back(std::move(*it));
		listToCheck.erase(it);

		// Update the parent pointer of the moved child
		parent = newParent;
	}
}

void GameObject::removeChild(GameObject* child)
{
	auto it = std::remove_if(childs.begin(), childs.end(), [child](const std::unique_ptr<GameObject>& ptr) {
		return ptr.get() == child;
		});

	// Check if the child was found
	if (it != childs.end())
	{
		// Erase the element at the end, which was moved there by std::remove_if
		childs.erase(it, childs.end());
		// The unique_ptr will automatically delete the removed child
	}
}

json GameObject::SaveInfo()
{
	json obj;

	if (parent) obj["Parent UUID"] = parent->UUID;

	json childArray;

	for (auto& go : childs)
	{
		childArray.push_back(go.get()->SaveInfo());
	}

	obj["Childs"] = childArray;

	json compArray;

	for (auto& comp : components)
	{
		compArray.push_back(comp.get()->SaveInfo());
	}

	// Change order of compArray --> [mesh before texture]

	obj["Components"] = compArray;

	obj["Active"] = isActive;
	obj["UUID"] = UUID;
	obj["Name"] = name;

	return obj;
}