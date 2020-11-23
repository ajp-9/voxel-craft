#pragma once

#include <vector>
#include <robin_hood.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "../../../Entities/Block/BlockTypes.h"
#include "../../../Entities/Block/RawBlock.h"
#include "../../../Utils/FreeHash.h"
#include "ChunkMesh.h"

class Chunk
{
	std::shared_ptr<ChunkMesh> mesh;
	std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>> blocksHash;
	//std::shared_ptr<robin_hood::unordered_map<glm::ivec3, std::tuple<unsigned char, unsigned char, bool>>> blocksHash1; //type, brightness, and isTransparent

	bool inBatch = true;
	short unsigned int generatedHeight = 0;
	bool ready = false; //for multi-threading

	std::vector<glm::ivec3> emitterBatchPositions;
	std::vector<glm::ivec3> eraseEmitterBatchPositions;

public:

	Chunk(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>> blocksHash, short unsigned int generatedHeight);
	Chunk() {}

	void addBlock(glm::ivec3 blockPos, unsigned char blockType)
	{
		generatedHeight = 0;
		if (blocksHash->find(blockPos) == blocksHash->end())
		{
			blocksHash->insert_or_assign(blockPos, blockType);
			if (blockType == block::torch)
			{
				emitterBatchPositions.push_back(blockPos);
			}
			inBatch = true;
		}
	}

	void removeblock(glm::ivec3 blockPos)
	{
		generatedHeight = 0;
		if (blocksHash->at(blockPos) == block::torch)
		{
			eraseEmitterBatchPositions.push_back(blockPos);
		}
		blocksHash->erase(blockPos);
		inBatch = true;
	}

	void setInBatch() { inBatch = true; }
	void setMesh(std::shared_ptr<ChunkMesh> mesh) { this->mesh = mesh; inBatch = false; ready = true; }

	//-------------------------

	std::shared_ptr<ChunkMesh>& getMesh() { return mesh; }
	std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>>& getBlocksHash() { return blocksHash; }
	bool getInBatch() { return inBatch; }
	bool getIsReady() { return ready; }
	int getGeneratedHeight() { return generatedHeight; }
	std::vector<glm::ivec3>& getEmitterBatch() { return emitterBatchPositions;}
	std::vector<glm::ivec3>& getEraseEmitterBatch() { return eraseEmitterBatchPositions; }

	~Chunk()
	{
		//printf("destroyed chunk\n");
		FreeHash::freeHash(blocksHash);
		mesh->~ChunkMesh();
	}
};