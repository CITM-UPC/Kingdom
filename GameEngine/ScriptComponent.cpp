#include "ScriptComponent.h"
#include "ScriptingEngine.h"
#include "GameObject.h"


ScriptComponent::ScriptComponent(GameObject* owner, std::string name) : Component(owner)
{
	//Mono thingies
	ScriptingEngine::SetCurrentUUID(owner->UUID);
	monoBehaviourInstance = ScriptingEngine::InstantiateClass("../ScriptingSandbox/bin/Debug/ScriptingSandbox.dll", "", name.c_str());
	ScriptingEngine::CleanCurrentUUID();
}

ScriptComponent::ScriptComponent(const ScriptComponent& other)
	: Component(other.owner),
	monoBehaviourInstance(other.monoBehaviourInstance)
{
}

ScriptComponent::~ScriptComponent()
{
	std::cout << "Deleting Script Component" << std::endl;
}

void ScriptComponent::Update()
{
	ScriptingEngine::UpdateScriptInstance(monoBehaviourInstance);
}

void ScriptComponent::Render() {}

json ScriptComponent::SaveInfo()
{
	json obj;
	return obj;
}