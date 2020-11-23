#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "../../World/Terrain/Chunk/Chunk.hpp"
#include "Light.h"

class LightManager
{
	//std::vector<LightEmitter> emitters;
	robin_hood::unordered_map<glm::ivec3, LightEmitter> emitters;
	robin_hood::unordered_map<glm::ivec3, unsigned char> lightsHash;
	bool hasAlteredLights = true;

public:
	LightManager() { }

	void addEmitter(glm::ivec3 emitterPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash);
	static void addLightsAround(glm::ivec3 lastPosition, unsigned char intensity, std::vector<std::pair<glm::ivec3, unsigned char>>& lastLights,
								std::vector<std::pair<glm::ivec3, unsigned char>>& lastLightsBeforeUpdate, robin_hood::unordered_map<glm::ivec3, unsigned char>& finishedLights,
						        robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash);
	void updateEmitters();

	void unloadChunksEmitters(glm::ivec2 chunkPosition);
	void eraseEmitter(glm::ivec3 position);

	robin_hood::unordered_map<glm::ivec3, unsigned char>& getLightsHash();
	robin_hood::unordered_map<glm::ivec3, LightEmitter>& getLightEmitters() { return emitters; }

	~LightManager()
	{

	}
};