#pragma once

#include <glm/vec3.hpp>

#include "BlockTypes.h"

struct RawBlock
{
	const short unsigned int ID;

	RawBlock(int blockID) : ID(blockID) {}
};