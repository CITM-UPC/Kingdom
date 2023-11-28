#include "Texture2D.h"
#include <GL/glew.h>
#include <IL/il.h>

#include <algorithm>

using namespace std;

Texture2D::Texture2D(std::shared_ptr<GameObject> owner, const std::string& path) : Component(owner) {
	//load image data using devil
	auto img = ilGenImage();
	ilBindImage(img);
	ilLoadImage(path.c_str());
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

Texture2D::Texture2D(Texture2D&& tex) noexcept : Component(tex.owner.lock()), _id(tex._id) {
	tex._id = 0;
}

Texture2D::~Texture2D() {
	if (_id) glDeleteTextures(1, &_id);
}

void Texture2D::Update()
{
}

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