#include "TerrainDecorator.hpp"

#include <iostream>

#include "../../../Entities/Block/BlockTypes.h"

TerrainDecorator::TerrainDecorator()
{

}

bool TerrainDecorator::setNGetIfTree(int seed, glm::ivec3 treePosition)
{
	std::srand(seed * treePosition.x * treePosition.y * treePosition.z);

	int oneOutOf = 125;

	if (std::rand() % oneOutOf == 1)
	{
		trees.push_back(treePosition);
		return true;
	}
	else
	{
		return false;
	}
}

bool TerrainDecorator::getIfFlower(int seed, glm::ivec3 flowerPosition)
{
	std::srand(seed * flowerPosition.x * flowerPosition.y * flowerPosition.z);

	int oneOutOf = 500;

	if (std::rand() % oneOutOf == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TerrainDecorator::addTrees(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>> blocksHash, StructureLoader loader, glm::ivec2 chunkPosition)
{
	for (auto& tree : trees)
	{
		for (auto& block : loader.getStructureTypes().at(structure::tree).getBlocks())
		{
			if (block.first.x + tree.x + (chunkPosition.x * 16) > (chunkPosition.x * 16) + 15)
			{
				waitingBlocks[glm::ivec2(chunkPosition.x + 1, chunkPosition.y)].
					push_back(std::make_pair(glm::ivec3(block.first.x + tree.x + (chunkPosition.x * 16), block.first.y + tree.y + (chunkPosition.y * 16), block.first.z + tree.z), block.second));
			}

			if (block.first.x + tree.x + (chunkPosition.x * 16) < chunkPosition.x * 16)
			{
				waitingBlocks[glm::ivec2(chunkPosition.x - 1, chunkPosition.y)].
					push_back(std::make_pair(glm::ivec3(block.first.x + tree.x + (chunkPosition.x * 16), block.first.y + tree.y + (chunkPosition.y * 16), block.first.z + tree.z), block.second));

			}

			if (block.first.y + tree.y + (chunkPosition.y * 16) > (chunkPosition.y * 16) + 15)
			{
				waitingBlocks[glm::ivec2(chunkPosition.x, chunkPosition.y + 1)].
					push_back(std::make_pair(glm::ivec3(block.first.x + tree.x + (chunkPosition.x * 16), block.first.y + tree.y + (chunkPosition.y * 16), block.first.z + tree.z), block.second));

			}

			if(block.first.y + tree.y + (chunkPosition.y * 16) < chunkPosition.y * 16)
			{
				waitingBlocks[glm::ivec2(chunkPosition.x, chunkPosition.y - 1)].
					push_back(std::make_pair(glm::ivec3(block.first.x + tree.x + (chunkPosition.x * 16), block.first.y + tree.y + (chunkPosition.y * 16), block.first.z + tree.z), block.second));

				continue;
			}

			blocksHash->insert_or_assign(glm::ivec3(block.first.x + tree.x + (chunkPosition.x * 16), block.first.y + tree.y + (chunkPosition.y * 16), block.first.z + tree.z), block.second);
		}
	}

	if (waitingBlocks.find(chunkPosition) != waitingBlocks.end())
	{
		for (auto& block : waitingBlocks.at(chunkPosition))
		{
			blocksHash->insert_or_assign(glm::ivec3(block.first.x, block.first.y, block.first.z), block.second);
		}

		waitingBlocks.erase(chunkPosition);
	}

	trees.clear();
}

void TerrainDecorator::addWaitingBlocks(glm::ivec2 chunkPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash)
{
	if (waitingBlocks.find(chunkPosition) != waitingBlocks.end())
	{
		for (auto& block : waitingBlocks.at(chunkPosition))
		{
			chunksHash.at(chunkPosition)->addBlock(block.first, block.second);
		}

		waitingBlocks.erase(chunkPosition);
	}
}