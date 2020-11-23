#pragma once

#include "../World/Terrain/Chunk/ChunkManager.hpp"
#include "Shader/Shader.h"
#include "../Entities/Player/Camera.hpp"

class CoreRenderer
{
public:
	CoreRenderer();

	void render_all(Camera& camera, Shader& shader, ChunkManager& manager, float timeLight, float width, float height);
};