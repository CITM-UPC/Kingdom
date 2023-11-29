#pragma once
#include "Component.h"
#include <string>
#include <memory>

#define CHECKERS_HEIGHT 100
#define CHECKERS_WIDTH  100

class Texture2D : public Component
{
private:
	unsigned int _id;
	unsigned int _id_checker;

public:
	Texture2D(GameObject* owner) : Component(owner), _id(0), _id_checker(0) {}

	Texture2D();

	explicit Texture2D(GameObject* owner, const std::string& path);
	Texture2D(Texture2D&& tex) noexcept; //move constructor (the one used when you call std::move)
	void bind() const;
	void unbind() const;
	void checker() const;
	~Texture2D();

	void Update() override;

	Type getType() const override {
		return Type::TEXTURE2D;
	}

	std::string path;
	int width, height;

private:

	Texture2D(const Texture2D&);
	Texture2D operator=(const Texture2D&) = delete;
};
