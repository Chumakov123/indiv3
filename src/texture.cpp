#include <texture.h>
#include <stdexcept>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
//#define STBI_ONLY_PNG
#include <stb_image.h>

Texture2D::Texture2D(const char* path) {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(path, &mWidth, &mHeight, &channel, 0);

	if (!image) {
		std::string error = "Не удалось загрузить изображение " + std::string(path);
		
		throw std::exception(error.c_str());
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	switch (channel) {
	case 4:
		format = GL_RGBA;
		break;
	case 3:
		format = GL_RGB;
		break;
	default:
		format = GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	//  std::cout << "Texture BASE (" << this << ") " << path << " created" << std::endl;
}

Texture2D::~Texture2D() {
	// std::cout << "Texture BASE (" << this << ")" << " deleted" << std::endl;
	glDeleteTextures(1, &textureID);
	textureID = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept {
	// std::cout << "Assignment-Move Texture2D (" << this << ") called " << std::endl;
	if (this != &texture) {
		glDeleteProgram(textureID);
		textureID = texture.textureID;
		format = texture.format;
		mWidth = texture.mWidth;
		mHeight = texture.mHeight;
		channel = texture.channel;

		texture.textureID = 0;

	}
	return *this;
}

Texture2D::Texture2D(Texture2D&& texture) noexcept {
	// std::cout << "Constructor-Move Texture2D (" << this << ") called " << std::endl;
	textureID = texture.textureID;
	format = texture.format;
	mWidth = texture.mWidth;
	mHeight = texture.mHeight;
	channel = texture.channel;

	texture.textureID = 0;
}

void Texture2D::bind() {
	if (textureID != 0) glBindTexture(GL_TEXTURE_2D, textureID);
	else std::cerr << " Texture not init " << std::endl;
}

void Texture2D::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}


int Texture2D::width() {
	return mHeight;
}

int Texture2D::height() {
	return mHeight;
}

const SubTexture& Texture2D::getSubTexture(const size_t& subTexName) {
	const static SubTexture defaultSubTexture;
	return defaultSubTexture;
}