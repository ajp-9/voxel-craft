#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "../RenderEngine/Textures/Texture.hpp"
#include "../RenderEngine/Shader/Shader.h"

class Model
{
	unsigned int VAO;
	unsigned int vertexCount;

public:
	Model(unsigned int VAO, unsigned int vertexCount)
	{
		this->VAO = VAO;
		this->vertexCount = vertexCount;
	}
	Model() { VAO = 0; vertexCount = 0; }

	void draw()
	{
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	}

	unsigned int getVAO() { return VAO; }
	unsigned int getVertexCount() { return vertexCount; }
};