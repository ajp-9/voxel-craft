#include "ChunkManager.hpp"

#include "ChunkMeshGenerator.hpp"

void ChunkManager::renderChunks(Shader& shader)
{
	for (auto& chunk : chunksHash)
	{
		if(chunk.second->getIsReady()) //making sure not in some render queue
			chunk.second->getMesh()->draw(shader);
	}
}

void ChunkManager::addChunk(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>>& blocksHash, glm::ivec2 chunkPosition, int generatedHeight)
{
	chunksHash[chunkPosition] = std::make_shared<Chunk>(blocksHash, generatedHeight);
	
	if(chunksHash.find(glm::ivec2(chunkPosition.x - 1, chunkPosition.y)) != chunksHash.end()) //adds surrounding chunks to mesh for building
		chunksHash[glm::ivec2(chunkPosition.x - 1, chunkPosition.y)]->setInBatch();

	if (chunksHash.find(glm::ivec2(chunkPosition.x + 1, chunkPosition.y)) != chunksHash.end())
		chunksHash[glm::ivec2(chunkPosition.x + 1, chunkPosition.y)]->setInBatch();

	if (chunksHash.find(glm::ivec2(chunkPosition.x, chunkPosition.y - 1)) != chunksHash.end())
		chunksHash[glm::ivec2(chunkPosition.x, chunkPosition.y - 1)]->setInBatch();

	if (chunksHash.find(glm::ivec2(chunkPosition.x, chunkPosition.y + 1)) != chunksHash.end())
		chunksHash[glm::ivec2(chunkPosition.x, chunkPosition.y + 1)]->setInBatch();
}

void ChunkManager::setChunkMeshFromQueue(std::vector<BlockType>& blockTypes, robin_hood::unordered_map<glm::ivec3, unsigned char>& lightsHash)
{
	int count = 5;
	for (auto& chunk : chunksHash)
	{
		if (chunk.second->getInBatch())
		{
			if (count)
			{
				chunk.second->setMesh(ChunkMeshGenerator::generateChunkMesh(chunk.second->getBlocksHash(), blockTypes, chunk.first, chunksHash, lightsHash, chunk.second->getGeneratedHeight()));
				count--;
			}
			else
			{
				break;
			}
		}
	}
}

void ChunkManager::batchChunk(glm::ivec2 chunkPosition)
{
	if (chunksHash.find(glm::ivec2(chunkPosition.x, chunkPosition.y)) != chunksHash.end()) //adds surrounding chunks to mesh for building
		chunksHash[glm::ivec2(chunkPosition.x, chunkPosition.y)]->setInBatch();
}

void ChunkManager::checkEmittersBatch(LightManager& lightManager)
{
	for (auto& chunk : chunksHash)
	{
		if (chunk.second->getEmitterBatch().size())
		{
			lightManager.addEmitter(chunk.second->getEmitterBatch()[0], chunksHash);
			chunk.second->getEmitterBatch().erase(chunk.second->getEmitterBatch().begin());
		}

		if (chunk.second->getEraseEmitterBatch().size())
		{
			lightManager.eraseEmitter(chunk.second->getEraseEmitterBatch()[0]);
			chunk.second->getEraseEmitterBatch().erase(chunk.second->getEraseEmitterBatch().begin());
		}
	}
}

void ChunkManager::checkIfBlockOutOfBounds(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>>& blocksHash, glm::ivec2 chunkPosition)
{
	robin_hood::unordered_map<glm::ivec3, unsigned char> blocksHashStack = *blocksHash.get(); // putting it into stack b/c heap doesnt like playing w hashes lol
	for (auto& block : blocksHashStack)
	{
		if (blocksHashStack.find(block.first) != blocksHashStack.end())
		{
			if (block.first.x > (chunkPosition.x * 16) + 15 ||
				block.first.y > (chunkPosition.y * 16) + 15 ||
				block.first.x < (chunkPosition.x * 16) ||
				block.first.y < (chunkPosition.y * 16))
			{
				blocksHash.get()->erase(block.first);
			}
		}
	}
}

void ChunkManager::unloadChunk(glm::ivec2 chunkPosition)
{
	chunksHash.erase(chunkPosition);
}