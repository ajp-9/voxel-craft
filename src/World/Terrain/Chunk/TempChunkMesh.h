#pragma once

#include <vector>
#include <glm/vec2.hpp>

struct TempChunkMesh
{
	std::vector<float> dataBuffer;
	std::vector<unsigned int> indicesBuffer;
	unsigned int vertexCount;
	glm::ivec2 chunkPosition;

	TempChunkMesh(std::vector<float> dataBuffer, std::vector<unsigned int> indicesBuffer, unsigned int vertexCount, glm::ivec2 chunkPosition)
		: dataBuffer(dataBuffer), indicesBuffer(indicesBuffer), vertexCount(vertexCount), chunkPosition(chunkPosition) {}

};