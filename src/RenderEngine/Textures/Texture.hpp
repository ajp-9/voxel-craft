#pragma once

#include <glad/glad.h>
#include <string>


class Texture
{
	unsigned int textureID;
public:
	Texture(std::string textureName);
	Texture() { textureID = 0; }

	void bind() { glBindTexture(GL_TEXTURE_2D, textureID); }
	unsigned int getTexture() { return textureID; }
};