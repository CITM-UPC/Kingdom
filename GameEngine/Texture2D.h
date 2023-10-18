#pragma once
#include <string>
#include <memory>

class Texture2D
{

private:
	unsigned int _id;

public:
	typedef std::shared_ptr<Texture2D> Ptr;
	explicit Texture2D(const std::string& path);
	Texture2D(Texture2D&& tex) noexcept;
	void bind() const;
	~Texture2D();

private:
	Texture2D(const Texture2D&);
	Texture2D operator=(const Texture2D&);

};

