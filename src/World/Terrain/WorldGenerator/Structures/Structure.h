#pragma once

#include <vector>
#include <glm/vec3.hpp>

class Structure
{
	unsigned char structureType;
	std::vector<std::pair<glm::ivec3, unsigned char>> blocks;

public:
	Structure(unsigned char structureType, std::vector<std::pair<glm::ivec3, unsigned char>> blocks)
		: structureType(structureType), blocks(blocks) {}

	std::vector<std::pair<glm::ivec3, unsigned char>> getBlocks() { return blocks; }
};