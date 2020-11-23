#pragma once

#include <vector>

#include "../../Utils/FreeHash.h"

struct Light
{
	unsigned char intensity;

	Light(unsigned char intensity) : intensity(intensity) {}
};

struct LightEmitter
{
	glm::ivec3 position;
	robin_hood::unordered_map<glm::ivec3, unsigned char> lights;

	LightEmitter(glm::ivec3 position, robin_hood::unordered_map<glm::ivec3, unsigned char> lights) : position(position), lights(lights) {}

	~LightEmitter()
	{
		FreeHash::freeHash(lights);
	}
};