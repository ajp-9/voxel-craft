#pragma once

#include "../../World/Terrain/Chunk/ChunkManager.hpp"
#include "Camera.hpp"

class Player : public Camera
{
	ChunkManager* manager = nullptr;
	unsigned char currentBlock = 0;

public:
	Player(glm::vec3 position) { this->setPosition(position); }

	void setCurrentBlock(unsigned char id);

	void breakBlock();
	void placeBlock();

	void setManager(ChunkManager& manager) { this->manager = &manager; }
};