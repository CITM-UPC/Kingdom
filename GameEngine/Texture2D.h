#pragma once
#include "Component.h"
#include <string>
#include <memory>

class Texture2D : public Component
{

private:
	unsigned int _id;

public:
	Texture2D() : _id(0) {}

	typedef std::shared_ptr<Texture2D> Ptr;
	explicit Texture2D(const std::string& path);
	Texture2D(Texture2D&& tex) noexcept;
	void bind() const;
	~Texture2D();

	void Update() override;

private:
	Texture2D(const Texture2D&);
	Texture2D operator=(const Texture2D&);

};

