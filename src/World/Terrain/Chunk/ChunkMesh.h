#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "../../../RenderEngine/Shader/Shader.h"
#include "../../../Utils/Math.h"

struct ChunkMesh
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int vertexCount;
	glm::mat4 transformationMatrix;

	ChunkMesh() { VAO = 0; VBO = 0; EBO = 0; vertexCount = 0; transformationMatrix = glm::mat4(0.0f); }

	ChunkMesh(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int vertexCount, glm::ivec2 chunkPosition)
		: VAO(VAO), VBO(VBO), EBO(EBO), vertexCount(vertexCount)
	{
		this->transformationMatrix = Math::createTransformationMatrix(glm::ivec3(0, 0, 0), glm::ivec3(0, 0, 0), 1.0f);
	}

	void draw(Shader& shader)
	{
		glBindVertexArray(VAO);
		shader.setMat4("model", transformationMatrix);
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	}

	~ChunkMesh()
	{
		//printf("destroyed mesh\n");
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
};