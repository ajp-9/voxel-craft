#include "LightManager.hpp"

#include "../../Utils/FreeHash.h"

void LightManager::addEmitter(glm::ivec3 emitterPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash)
{
	//glm::ivec2 lastChunk(std::ceil((lastPosition.x + .5) / 16) - 1, std::ceil((lastPosition.y + .5) / 16) - 1); //get lights current position

	int intensityIterator = 100;

	glm::vec3 emitterPositionF = emitterPosition;

	robin_hood::unordered_map<glm::ivec3, unsigned char> finishedLights;

	std::vector<std::pair<glm::ivec3, unsigned char>> lastLights;

	bool first = true;
	for (intensityIterator; intensityIterator > 0; intensityIterator -= 10) // front(0) -> front_right(1) -> back(2) -> front_left(3) -> top(4) -> bottom(5)
	{
		if (!first)
		{
			std::vector<std::pair<glm::ivec3, unsigned char>> lastLightsBeforeUpdate;

			for (auto& light : lastLights)
			{
				//addLightsAround(light.first, light.second, lastLights, lastLightsBeforeUpdate, finishedLights, chunksHash);
			}
		}
		else // first iteration thru
		{
			glm::ivec2 tempChunkPos(std::ceil(emitterPositionF.x / 16) - 1, std::ceil((emitterPositionF.y - 1) / 16) - 1);
			if (chunksHash.find(glm::ivec2(tempChunkPos)) != chunksHash.end())
			{
				auto frontBlockHash = *chunksHash.at(tempChunkPos)->getBlocksHash();
				if (frontBlockHash.find(glm::ivec3(emitterPosition.x, emitterPosition.y - 1, emitterPosition.z)) == frontBlockHash.end()) // front
				{
					lastLights.push_back(std::make_pair(glm::ivec3(emitterPosition.x, emitterPosition.y - 1, emitterPosition.z), intensityIterator));
				}
			}

			tempChunkPos = glm::ivec2(std::ceil((emitterPositionF.x + 1) / 16) - 1, std::ceil(emitterPositionF.y / 16) - 1);
			if (chunksHash.find(glm::ivec2(tempChunkPos)) != chunksHash.end())
			{
				auto frontBlockHash = *chunksHash.at(tempChunkPos)->getBlocksHash();
				if (frontBlockHash.find(glm::ivec3(emitterPosition.x + 1, emitterPosition.y, emitterPosition.z)) == frontBlockHash.end()) // front_right
				{
					lastLights.push_back(std::make_pair(glm::ivec3(emitterPosition.x + 1, emitterPosition.y, emitterPosition.z), intensityIterator));
				}
			}

			tempChunkPos = glm::ivec2(std::ceil(emitterPositionF.x / 16) - 1, std::ceil((emitterPositionF.y + 1) / 16) - 1);
			if (chunksHash.find(glm::ivec2(tempChunkPos)) != chunksHash.end())
			{
				auto frontBlockHash = *chunksHash.at(tempChunkPos)->getBlocksHash();
				if (frontBlockHash.find(glm::ivec3(emitterPosition.x, emitterPosition.y + 1, emitterPosition.z)) == frontBlockHash.end()) // back
				{
					lastLights.push_back(std::make_pair(glm::ivec3(emitterPosition.x, emitterPosition.y + 1, emitterPosition.z), intensityIterator));
				}
			}

			tempChunkPos = glm::ivec2(std::ceil((emitterPositionF.x - 1) / 16) - 1, std::ceil(emitterPositionF.y / 16) - 1);
			if (chunksHash.find(glm::ivec2(tempChunkPos)) != chunksHash.end())
			{
				auto frontBlockHash = *chunksHash.at(tempChunkPos)->getBlocksHash();
				if (frontBlockHash.find(glm::ivec3(emitterPosition.x - 1, emitterPosition.y, emitterPosition.z)) == frontBlockHash.end()) // front_left
				{
					lastLights.push_back(std::make_pair(glm::ivec3(emitterPosition.x - 1, emitterPosition.y, emitterPosition.z), intensityIterator));
				}
			}

			tempChunkPos = glm::ivec2(std::ceil(emitterPositionF.x / 16) - 1, std::ceil(emitterPositionF.y / 16) - 1);
			if (chunksHash.find(glm::ivec2(tempChunkPos)) != chunksHash.end())
			{
				auto frontBlockHash = *chunksHash.at(tempChunkPos)->getBlocksHash();
				if (frontBlockHash.find(glm::ivec3(emitterPosition.x, emitterPosition.y, emitterPosition.z + 1)) == frontBlockHash.end()) // top
				{
					lastLights.push_back(std::make_pair(glm::ivec3(emitterPosition.x, emitterPosition.y, emitterPosition.z + 1), intensityIterator));
				}
			}

			tempChunkPos = glm::ivec2(std::ceil(emitterPositionF.x / 16) - 1, std::ceil(emitterPositionF.y / 16) - 1);
			if (chunksHash.find(glm::ivec2(tempChunkPos)) != chunksHash.end())
			{
				auto frontBlockHash = *chunksHash.at(tempChunkPos)->getBlocksHash();
				if (frontBlockHash.find(glm::ivec3(emitterPosition.x, emitterPosition.y, emitterPosition.z - 1)) == frontBlockHash.end()) // bottom
				{
					lastLights.push_back(std::make_pair(glm::ivec3(emitterPosition.x, emitterPosition.y, emitterPosition.z - 1), intensityIterator));
				}
			}
			
			first = false;
		}
	}
	// when done there is lastLights already done so add them to the hash last
	for (auto& light : lastLights)
	{
		finishedLights[light.first] = light.second;
	}

	finishedLights[emitterPosition] = 100;
	hasAlteredLights = true;

	emitters.insert_or_assign(emitterPosition, LightEmitter(emitterPosition, finishedLights));
}

void LightManager::addLightsAround(glm::ivec3 lastPosition, unsigned char intensity, std::vector<std::pair<glm::ivec3, unsigned char>>& lastLights,
								   std::vector<std::pair<glm::ivec3, unsigned char>>& lastLightsBeforeUpdate, robin_hood::unordered_map<glm::ivec3, unsigned char>& finishedLights,
								   robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash)
{
	glm::ivec2 tempChunkPos(std::ceil(lastPosition.x / 16) - 1, std::ceil((lastPosition.y - 1) / 16) - 1);
	if (chunksHash.find(glm::ivec2(tempChunkPos)) != chunksHash.end())
	{
		auto frontBlockHash = *chunksHash.at(tempChunkPos)->getBlocksHash();
		if (frontBlockHash.find(glm::ivec3(lastPosition.x, lastPosition.y - 1, lastPosition.z)) == frontBlockHash.end()) // front
		{
			lastLights.push_back(std::make_pair(glm::ivec3(lastPosition.x, lastPosition.y - 1, lastPosition.z), intensity));
		}
	}

}

void LightManager::updateEmitters()
{
	robin_hood::unordered_map<glm::ivec3, unsigned char> lightsBuffer;

	for (auto& emitter : emitters)
	{
		for (auto& light : emitter.second.lights)
		{
			if (lightsBuffer.find(light.first) != emitter.second.lights.end()) // if finds a light already there then just checks if its stronger than the other
			{
				if (lightsBuffer[light.first] >= light.second)
				{
					continue;
				}
				else
				{
					lightsBuffer.at(light.first) = light.second;
				}
			}

			lightsBuffer[light.first] = light.second;
		}
	}

	FreeHash::freeHash(lightsHash); // frees hash, JIC tho
	lightsHash = lightsBuffer;
	hasAlteredLights = false;
}

void LightManager::unloadChunksEmitters(glm::ivec2 chunkPosition)
{
	for(auto& emitter : emitters)
	{
		glm::ivec2 emitterPosition(std::ceil((emitter.first.x + .5) / 16) - 1, std::ceil((emitter.first.y + .5) / 16) - 1);
		if (emitterPosition == chunkPosition)
		{
			emitters.erase(emitter.first);
			hasAlteredLights = true;
		}
	}
}

void LightManager::eraseEmitter(glm::ivec3 position)
{
	emitters.erase(position);
	hasAlteredLights = true;
}

robin_hood::unordered_map<glm::ivec3, unsigned char>& LightManager::getLightsHash()
{
	if (hasAlteredLights) // doesnt have to regenerate all lights if it doesnt have to.
	{
		updateEmitters();
	}

	return lightsHash;
}