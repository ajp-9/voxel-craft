#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp> //

#include <vector>
#include <glm/vec3.hpp>
#include <memory>

#include "../Chunk/Chunk.hpp"
#include "Structures/StructureLoader.hpp"

class TerrainDecorator
{
	std::vector<glm::ivec3> trees;
	std::vector<glm::ivec3> flowers;

	robin_hood::unordered_map<glm::ivec2, std::vector<std::pair<glm::ivec3, unsigned char>>> waitingBlocks;

public:
	TerrainDecorator();

	bool setNGetIfTree(int seed, glm::ivec3 treePosition);
	bool getIfFlower(int seed, glm::ivec3 flowerPosition);
	void addTrees(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>> blocksHash, StructureLoader loader, glm::ivec2 chunkPosition);
	void addWaitingBlocks(glm::ivec2 chunkPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash);

	robin_hood::unordered_map<glm::ivec2, std::vector<std::pair<glm::ivec3, unsigned char>>> getWaitingBlocks() { return waitingBlocks; };
};