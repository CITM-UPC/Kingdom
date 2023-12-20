#include "Texture2D.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <IL/il.h>
#include <iostream>
#include <algorithm>

using namespace std;

Texture2D::Texture2D(GameObject* owner, const std::string& path) : Component(owner) {
	//load image data using devil

	auto img = ilGenImage();
	ilBindImage(img);

	// if path ends with .dds, [ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);]

	ilLoadImage(path.c_str());
	this->path = path;
	auto width = ilGetInteger(IL_IMAGE_WIDTH);
	this->width = static_cast<int>(width);
	auto height = ilGetInteger(IL_IMAGE_HEIGHT);
	this->height = static_cast<int>(height);
	auto channels = ilGetInteger(IL_IMAGE_CHANNELS);
	auto data = ilGetData();

	//load image as a texture in VRAM
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	size_t lastOf = path.find_last_of('/');
	this->fileName = path.substr(lastOf + 1);
	lastOf = this->fileName.find_last_of('.');
	this->fileName = this->fileName.substr(0, lastOf);
	this->fileName = "Library/Materials/" + this->fileName + ".dds";

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	ilSave(IL_DDS, this->fileName.c_str());

	//now we can delete image from RAM
	ilDeleteImage(img);

	// Checker
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &_id_checker);
	glBindTexture(GL_TEXTURE_2D, _id_checker);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
}

Texture2D::Texture2D(Texture2D&& tex) noexcept :
	Component(tex.owner),
	_id(tex._id),
	_id_checker(tex._id_checker),
	path(tex.path),
	width(tex.width),
	height(tex.height),
	fileName(tex.fileName)
{
	tex._id = 0;
	tex._id_checker = 0;
}

Texture2D::Texture2D(const Texture2D& cpy) :
	Component(cpy.owner),
	_id(cpy._id),
	_id_checker(cpy._id_checker),
	path(cpy.path),
	width(cpy.width),
	height(cpy.height),
	fileName(cpy.fileName)
{
	const_cast<Texture2D&>(cpy)._id = 0;
	const_cast<Texture2D&>(cpy)._id_checker = 0;
}

Texture2D::~Texture2D() {
	if (_id_checker) glDeleteTextures(1, &_id_checker);

	if (_id) glDeleteTextures(1, &_id);
}

void Texture2D::Update() {}

void Texture2D::Render() {}

void Texture2D::bind() const {
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture2D::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::checker() const
{
	glBindTexture(GL_TEXTURE_2D, _id_checker);
}

json Texture2D::SaveInfo()
{
	json obj;

	obj["Owner"] = owner->UUID;
	obj["Binary Path"] = fileName;
	obj["Type"] = static_cast<int>(getType());

	return obj;
}