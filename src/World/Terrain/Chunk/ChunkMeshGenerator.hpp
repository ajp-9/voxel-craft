#pragma once

#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Chunk.hpp"

struct ChunkMeshGenerator
{
    static std::shared_ptr<ChunkMesh> generateChunkMesh(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>>& blocksHash,
                                                        std::vector<BlockType>& blockTypes,
                                                        glm::ivec2 chunkPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash,
                                                        robin_hood::unordered_map<glm::ivec3, unsigned char>& lightsHash,
                                                        int generatedHeight);

private:
    inline static void addFlowerToMesh(glm::ivec3 first, unsigned char second, std::vector<BlockType>& blockTypes,
        robin_hood::unordered_map<glm::ivec3, unsigned char> lightsHash,
        unsigned int& vertexCount, int& indicesRound, std::vector<float>& dataBuffer,
        std::vector<unsigned int>& indices);
    inline static void addTorchToMesh(glm::ivec3 first, unsigned char second, std::vector<BlockType>& blockTypes,
        unsigned int& vertexCount, int& indicesRound, std::vector<float>& dataBuffer,
        std::vector<unsigned int>& indices);
    inline static void addBlockToMesh(glm::ivec3 first, unsigned char second, std::vector<BlockType>& blockTypes,
        robin_hood::unordered_map<glm::ivec3, unsigned char>& blocksHash,
        robin_hood::unordered_map<glm::ivec3, unsigned char>& lightsHash,
        glm::ivec2& chunkPosition,
        robin_hood::unordered_map<glm::ivec3, unsigned char>& frontChunkHash,
        robin_hood::unordered_map<glm::ivec3, unsigned char>& front_rightChunkHash,
        robin_hood::unordered_map<glm::ivec3, unsigned char>& backChunkHash,
        robin_hood::unordered_map<glm::ivec3, unsigned char>& front_leftChunkHash,
        unsigned int& vertexCount, int& indicesRound, std::vector<float>& dataBuffer,
        std::vector<unsigned int>& indices);
};