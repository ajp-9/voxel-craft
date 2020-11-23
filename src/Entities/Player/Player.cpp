#include "Player.hpp"

void Player::setCurrentBlock(unsigned char id)
{
	currentBlock = id;
}

void Player::breakBlock()
{
	robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> tempChunksHash = manager->getChunksHash();

	glm::vec3 rayPosition = getPosition();
	for (int i = 0; i < 75; i++)
	{
		rayPosition += getFront() * .1f;

		glm::ivec2 chunkTemp(std::ceil((rayPosition.x + .5f) / 16) - 1, std::ceil((rayPosition.y + .5f) / 16) - 1);

		if (tempChunksHash.find(chunkTemp) == tempChunksHash.end())
		{
			continue; // makes sure that if it doesnt exit it isnt tested
		}

		glm::ivec3 blockTemp(std::round(rayPosition.x), std::round(rayPosition.y), std::round(rayPosition.z));

		if (tempChunksHash.at(chunkTemp)->getBlocksHash()->find(blockTemp) != tempChunksHash.at(chunkTemp)->getBlocksHash()->end())
		{
			if (blockTemp.x == chunkTemp.x * 16)
				manager->batchChunk(glm::ivec2(chunkTemp.x - 1, chunkTemp.y));
			if (blockTemp.x == (chunkTemp.x * 16) + 15)
				manager->batchChunk(glm::ivec2(chunkTemp.x + 1, chunkTemp.y));
			if (blockTemp.y == chunkTemp.y * 16)
				manager->batchChunk(glm::ivec2(chunkTemp.x, chunkTemp.y - 1));
			if (blockTemp.y == (chunkTemp.y * 16) + 15)
				manager->batchChunk(glm::ivec2(chunkTemp.x, chunkTemp.y + 1));

			manager->getChunksHash().at(chunkTemp)->removeblock(blockTemp);
			break;
		}
	}
}

void Player::placeBlock()
{
	robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> tempChunksHash = manager->getChunksHash();

	glm::vec3 rayPosition = getPosition();
	glm::vec3 lastPosition = rayPosition;
	for (int i = 0; i < 75; i++)
	{
		rayPosition += getFront() * .1f;
		
		glm::ivec2 currentChunk(std::ceil((rayPosition.x + .5) / 16) - 1, std::ceil((rayPosition.y + .5) / 16) - 1);
		glm::ivec2 lastChunk(std::ceil((lastPosition.x + .5) / 16) - 1, std::ceil((lastPosition.y + .5) / 16) - 1);

		if (tempChunksHash.find(lastChunk) == tempChunksHash.end())
		{
			continue; // makes sure that if it doesnt exit it isnt tested
		}

		glm::ivec3 blockTemp(std::round(rayPosition.x), std::round(rayPosition.y), std::round(rayPosition.z));
		glm::ivec3 lastBlockTemp(std::round(lastPosition.x), std::round(lastPosition.y), std::round(lastPosition.z));

		if (tempChunksHash.at(currentChunk)->getBlocksHash()->find(blockTemp) != tempChunksHash.at(currentChunk)->getBlocksHash()->end())
		{
			if (lastBlockTemp.x == lastChunk.x * 16)
				manager->batchChunk(glm::ivec2(lastChunk.x - 1, lastChunk.y));
			if (lastBlockTemp.x == (lastChunk.x * 16) + 15)
				manager->batchChunk(glm::ivec2(lastChunk.x + 1, lastChunk.y));
			if (lastBlockTemp.y == lastChunk.y * 16)
				manager->batchChunk(glm::ivec2(lastChunk.x, lastChunk.y - 1));
			if (lastBlockTemp.y == (lastChunk.y * 16) + 15)
				manager->batchChunk(glm::ivec2(lastChunk.x, lastChunk.y + 1));

			manager->getChunksHash().at(lastChunk)->addBlock(lastBlockTemp, currentBlock);
			break;
		}

		lastPosition = rayPosition;
	}
}