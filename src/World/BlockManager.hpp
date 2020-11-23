#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

#include "../Entities/Block/BlockTypes.h"
#include "../Entities/Model.h"
#include "../Entities/Block/RawBlock.h"

class BlockManager
{
	std::vector<BlockType> blockTypes;

public:

	void loadBlockTypes();

	BlockType loadBlockType(float uvs[]);
	BlockType loadTorchType(float uvs[]);
	BlockType loadFlowerType(float uvs[]);

	std::vector<BlockType>& getBlockTypes() { return blockTypes; }
};