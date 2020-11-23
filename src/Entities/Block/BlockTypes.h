#pragma once

#include <vector>

#include "../../RenderEngine/Textures/TextureAtlas.hpp"

namespace block
{
	enum
	{
		grass,
		dirt,
		stone,
		sand,
		gravel,
		wood,
		leaves,
		planks,
		water,
		glass,
		cobblestone,
		bedrock,
		rose,
		torch,
		coal_ore,
		iron_ore,
		gold_ore,
		diamond_ore,
		emerald_ore,
		unknown
	};
}

struct BlockType
{
	const std::vector<float> packagedBufferData;

	const short int vertexCount = 72;

	BlockType(std::vector<float> packagedBufferData) : packagedBufferData(packagedBufferData) {}
};

struct Grass
{
	unsigned char ID = block::grass;
	float* uvs;

	Grass()
	{
		uvs = TextureAtlas::getUVCoords(0, 15, 1, 15, 2, 15);
	}
};

struct Dirt
{
	unsigned char ID = block::dirt;
	float* uvs;

	Dirt()
	{
		uvs = TextureAtlas::getUVCoords(2, 15, 2, 15, 2, 15);
	}
};

struct Stone
{
	unsigned char ID = block::stone;
	float* uvs;

	Stone()
	{
		uvs = TextureAtlas::getUVCoords(6, 15, 6, 15, 6, 15);
	}
};

struct Sand
{
	unsigned char ID = block::sand;
	float* uvs;

	Sand()
	{
		uvs = TextureAtlas::getUVCoords(0, 14, 0, 14, 0, 14);
	}
};

struct Gravel
{
	unsigned char ID = block::gravel;
	float* uvs;

	Gravel()
	{
		uvs = TextureAtlas::getUVCoords(1, 14, 1, 14, 1, 14);
	}
};

struct Wood
{
	unsigned char ID = block::wood;
	float* uvs;

	Wood()
	{
		uvs = TextureAtlas::getUVCoords(4, 15, 3, 15, 4, 15);
	}
};

struct Leaves
{
	unsigned char ID = block::leaves;
	float* uvs;

	Leaves()
	{
		uvs = TextureAtlas::getUVCoords(5, 15, 5, 15, 5, 15);
	}
};

struct Planks
{
	unsigned char ID = block::planks;
	float* uvs;

	Planks()
	{
		uvs = TextureAtlas::getUVCoords(3, 14, 3, 14, 3, 14);
	}
};

struct Water
{
	unsigned char ID = block::water;
	float* uvs;

	Water()
	{
		uvs = TextureAtlas::getUVCoords(2, 14, 2, 14, 2, 14);
	}
};

struct Glass
{
	unsigned char ID = block::glass;
	float* uvs;

	Glass()
	{
		uvs = TextureAtlas::getUVCoords(3, 13, 3, 13, 3, 13);
	}
};

struct Cobblestone
{
	unsigned char ID = block::cobblestone;
	float* uvs;

	Cobblestone()
	{
		uvs = TextureAtlas::getUVCoords(6, 14, 6, 14, 6, 14);
	}
};

struct Bedrock
{
	unsigned char ID = block::bedrock;
	float* uvs;

	Bedrock()
	{
		uvs = TextureAtlas::getUVCoords(7, 14, 7, 14, 7, 14);
	}
};

struct Rose
{
	unsigned char ID = block::rose;
	float* uvs;

	Rose()
	{
		uvs = TextureAtlas::getUVCoords(15, 15, 15, 15, 15, 15);
	}
};

struct Torch
{
	unsigned char ID = block::torch;

	float* uvs;

	Torch()
	{
		//special shit goes here or something idk
		uvs = TextureAtlas::getUVCoords(0, 0, 0, 0, 0, 0);
	}
};

struct Unknown
{
	unsigned char ID = block::unknown;
	float* uvs;

	Unknown()
	{
		uvs = TextureAtlas::getUVCoords(14, 0, 14, 0, 14, 0);
	}
};