#pragma once

#include <vector>/

#include "../Chunk/ChunkManager.hpp"
#include "Structures/StructureLoader.hpp"
#include "../../../Entities/Light/LightManager.hpp"
#include "TerrainDecorator.hpp"

class TerrainGenerator
{
	bool ready = true;
	std::vector<glm::ivec2> generateQueue;
	int WORLD_SIZE = 12; //world size(for unloading chunks) needs to be bigger than the loading chunk size
	int LOAD_SIZE = 10;
	glm::ivec2 lastPlayerPosition;
	TerrainDecorator decorator;
	StructureLoader loader;
	LightManager lightManager;

public:
	TerrainGenerator();
	
	void generate(ChunkManager& manager, std::vector<BlockType>& blockTypes, glm::vec3 playerPosition);

private:
	void loadFromQueue(ChunkManager& manager, std::vector<BlockType>& blockTypes);
	void generateChunk(ChunkManager& manager, glm::ivec2 chunkPosition, std::vector<BlockType>& blockTypes);
	void loadToQueue(glm::ivec2 playerChunkPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash);

public:
	int getWORLD_SIZE() { return WORLD_SIZE; }
};