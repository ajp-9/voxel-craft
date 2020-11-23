#include "BlockManager.hpp"

#include <fstream>
#include <iostream>

#include "../Utils/Math.h"
#include "../Entities/Block/BlockPositions.h"
#include "../RenderEngine/Textures/TextureAtlas.hpp"

void BlockManager::loadBlockTypes() // in the order of the enums
{
	Grass grassRaw;
    blockTypes.push_back(loadBlockType(grassRaw.uvs)); // needs to be in order as enums

    Dirt dirtRaw;
    blockTypes.push_back(loadBlockType(dirtRaw.uvs));

    Stone stoneRaw;
    blockTypes.push_back(loadBlockType(stoneRaw.uvs));

	Sand sand;
	blockTypes.push_back(loadBlockType(sand.uvs));

	Gravel gravel;
	blockTypes.push_back(loadBlockType(gravel.uvs));

	Wood wood;
	blockTypes.push_back(loadBlockType(wood.uvs));

	Leaves leaves;
	blockTypes.push_back(loadBlockType(leaves.uvs));

	Planks planks;
	blockTypes.push_back(loadBlockType(planks.uvs));

	Water water;
	blockTypes.push_back(loadBlockType(water.uvs));

	Glass glass;
	blockTypes.push_back(loadBlockType(glass.uvs));

	Cobblestone cobblestone;
	blockTypes.push_back(loadBlockType(cobblestone.uvs));

	Bedrock bedrock;
	blockTypes.push_back(loadBlockType(bedrock.uvs));

	Rose rose;
	blockTypes.push_back(loadFlowerType(rose.uvs));

	Torch torch;
	blockTypes.push_back(loadTorchType(torch.uvs));

	Unknown unknown;
	blockTypes.push_back(loadBlockType(unknown.uvs));

}

BlockType BlockManager::loadBlockType(float uvs[])
{
	std::vector<float> bufferData;

	int posCount = 0;
	int texCount = 0;
	int brightCount = 0;
	for (int i = 0; i < (sizeof(vertices) / sizeof(vertices[0])) / 3; i++)
	{
		bufferData.push_back(vertices[posCount]);
		posCount++;
		bufferData.push_back(vertices[posCount]);
		posCount++;
		bufferData.push_back(vertices[posCount]);
		posCount++;
		bufferData.push_back(uvs[texCount]);
		texCount++;
		bufferData.push_back(uvs[texCount]);
		texCount++;
		bufferData.push_back(brightness[brightCount]);
		brightCount++;
	}

	/*for (auto& b : bufferData)
	{
		std::cout << b << std::endl;
	}*/

	return BlockType(bufferData);
}

BlockType BlockManager::loadTorchType(float uvs[])
{

	std::vector<float> data;

	for (auto& d : packagedTorchData)
	{
		data.push_back(d);
	}

	return BlockType(data);
}

BlockType BlockManager::loadFlowerType(float uvs[])
{
	std::vector<float> bufferData;

	int posCount = 0;
	int texCount = 0;
	int brightCount = 0;

	for (int i = 0; i < (sizeof(flowerVertices) / sizeof(flowerVertices[0])) / 3; i++)
	{
		bufferData.push_back(flowerVertices[posCount]);
		posCount++;
		bufferData.push_back(flowerVertices[posCount]);
		posCount++;
		bufferData.push_back(flowerVertices[posCount]);
		posCount++;
		bufferData.push_back(uvs[texCount]);
		texCount++;
		bufferData.push_back(uvs[texCount]);
		texCount++;
		bufferData.push_back(100000.0f);
		brightCount++;
	}

	return BlockType(bufferData);
}