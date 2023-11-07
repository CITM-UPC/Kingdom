#pragma once
#include "Component.h"
#include <string>
#include <memory>

class Texture2D : public Component
{
private:
	unsigned int _id;

public:
	Texture2D(GameObject& owner) : Component(owner), _id(0) {}

	explicit Texture2D(GameObject& owner, const std::string& path);
	Texture2D(Texture2D&& tex) noexcept; //move constructor (the one used when you call std::move)
	void bind() const;
	~Texture2D();

	void Update() override;

	Type getType() const override {
		return Type::TEXTURE2D;
	}

private:
	Texture2D(const Texture2D&);
	Texture2D operator=(const Texture2D&) = delete;
};
