#pragma once
#include "Component.h"
#include "..\mono\include\mono\jit\jit.h"

class ScriptComponent : public Component
{
public:

	ScriptComponent(GameObject* owner, std::string name);
	ScriptComponent(const ScriptComponent& other);

	~ScriptComponent();

	void Update() override;

	void Render() override; //Ignore this

	Type getType() const override {
		return Type::SCRIPT;
	}

	json SaveInfo();

	MonoObject* monoBehaviourInstance;
};

