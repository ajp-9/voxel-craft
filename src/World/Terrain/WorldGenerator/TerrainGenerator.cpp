#include "TerrainGenerator.hpp"

#include <GLFW/glfw3.h>
#include <thread>

#include "../Chunk/ChunkMeshGenerator.hpp"
#include "PerlinNoise.h"

#define BLOCK_POS glm::ivec3(x + (chunkPosition.x * 16), y + (chunkPosition.y * 16), z)

TerrainGenerator::TerrainGenerator()
{

}

void TerrainGenerator::generate(ChunkManager& manager, std::vector<BlockType>& blockTypes, glm::vec3 playerPosition)
{
    manager.checkEmittersBatch(lightManager);

    manager.setChunkMeshFromQueue(blockTypes, lightManager.getLightsHash());

    if (generateQueue.size()) //checks size if greater than zero
    {
        std::thread loadFromQueueThread(&TerrainGenerator::loadFromQueue, this, std::ref(manager), std::ref(blockTypes));
        loadFromQueueThread.join();
    }
    else
    {
        const glm::ivec2 playerChunkPosition(std::ceil((playerPosition.x + .5) / 16) - 1, std::ceil((playerPosition.y + .5) / 16) - 1);
        const glm::ivec3 playerBlockPosition(std::round(playerPosition.x), std::round(playerPosition.y), std::round(playerPosition.z));

        //std::thread loadToQueueThread(&TerrainGenerator::loadToQueue, this, std::ref(playerChunkPosition), std::ref(manager.getChunksHash())); // hashes like to be broken by shard ptrs
        loadToQueue(playerChunkPosition, manager.getChunksHash());

        if (decorator.getWaitingBlocks().size())
        {
            auto waitingBlocksHash = decorator.getWaitingBlocks(); // b/c it wont any other way-thats why
            auto chunksHashStack = manager.getChunksHash(); // b/c it wont any other way-thats why

            for (auto& chunk : waitingBlocksHash)
            {
                if (chunksHashStack.find(chunk.first) != chunksHashStack.end())
                {
                    if (chunksHashStack.at(chunk.first)->getIsReady())
                    {
                        decorator.addWaitingBlocks(chunk.first, manager.getChunksHash());
                    }
                }
            }
        }

        if (lastPlayerPosition != playerChunkPosition) // only unloads when player leaves current chunk
        {
            robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> chunksHashStack = manager.getChunksHash(); // b/c it wont any other way-thats why
            for (auto& chunk : chunksHashStack)
            {
                if (chunk.second->getIsReady())
                {
                    if (chunk.first.x - WORLD_SIZE >= playerChunkPosition.x ||
                        chunk.first.x + WORLD_SIZE <= playerChunkPosition.x ||
                        chunk.first.y - WORLD_SIZE >= playerChunkPosition.y ||
                        chunk.first.y + WORLD_SIZE <= playerChunkPosition.y)
                    {
                        manager.unloadChunk(chunk.first);
                        lightManager.unloadChunksEmitters(chunk.first);
                    }
                }
            }
        }

        lastPlayerPosition = playerChunkPosition;
    }
}

void TerrainGenerator::loadFromQueue(ChunkManager& manager, std::vector<BlockType>& blockTypes)
{
    if (generateQueue.size())
    {
        generateChunk(manager, generateQueue[0], blockTypes);
        generateQueue.erase(generateQueue.begin(), generateQueue.begin() + 1);
    }
}

void TerrainGenerator::generateChunk(ChunkManager& manager, glm::ivec2 chunkPosition, std::vector<BlockType>& blockTypes)
{   
    int seed = 2;
    int seaLevel = 20;

    std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>> blocksHash = std::make_shared<robin_hood::unordered_map<glm::ivec3, unsigned char>>();

    int generatedHeight = 999;

    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            int grass = 1;
            int dirt = 2;
            int sand = 0;
            int water = 0;

            int z = PerlinNoise::getPerlinNoise(seed, x + (chunkPosition.x * 16) + 999999, y + (chunkPosition.y * 16) + 999999) + 30;

            if (z == seaLevel || z == seaLevel - 2|| z == seaLevel - 1 || z == seaLevel + 1 || z == seaLevel + 2)
            {
                sand = 2;
                dirt--;
                grass = 0;
            }

            if (z < seaLevel)
            {
                water = seaLevel - z;
                z += water;
                grass = 0;
            }

            for (z; z > 0; z--)
            {
                if (z == 1) // checks if bedrock
                {
                    blocksHash->insert_or_assign(BLOCK_POS, block::bedrock);
                    continue;
                }

                if (water)
                {
                    blocksHash->insert_or_assign(BLOCK_POS, block::water);
                    water--;
                    continue;
                }

                if (sand)
                {
                    blocksHash->insert_or_assign(BLOCK_POS, block::sand);
                    sand--;
                    continue;
                }

                if (grass)
                {
                    if (!decorator.setNGetIfTree(seed, glm::ivec3(x, y, z)))
                    {
                        blocksHash->insert_or_assign(BLOCK_POS, block::grass);
                        if (decorator.getIfFlower(seed, glm::ivec3(x, y, z + 1)))
                            blocksHash->insert_or_assign(glm::ivec3(x + (chunkPosition.x * 16), y + (chunkPosition.y * 16), z + 1), block::rose);
                    }

                    grass--;
                    continue;
                }

                if (dirt)
                {
                    blocksHash->insert_or_assign(BLOCK_POS, block::dirt);
                    dirt--;

                    if (generatedHeight > z)
                    {
                        generatedHeight = z;
                    }

                    continue;
                }

                blocksHash->insert_or_assign(BLOCK_POS, block::stone);
            }
        }
    }

    decorator.addTrees(blocksHash, loader, chunkPosition);

    manager.checkIfBlockOutOfBounds(blocksHash, chunkPosition);

    manager.addChunk(blocksHash, chunkPosition, generatedHeight);
}

void TerrainGenerator::loadToQueue(glm::ivec2 playerChunkPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash)
{
    std::vector<glm::ivec2> tempQueue;
    for (int x = -LOAD_SIZE + playerChunkPosition.x; x < LOAD_SIZE + playerChunkPosition.x; x++)
    {
        for (int y = -LOAD_SIZE + playerChunkPosition.y; y < LOAD_SIZE + playerChunkPosition.y; y++)
        {
            if(chunksHash.find(glm::ivec2(x, y)) == chunksHash.end())
                tempQueue.push_back(glm::ivec2(x, y));
        }
    }

    generateQueue.insert(generateQueue.begin(), tempQueue.begin(), tempQueue.end());
}