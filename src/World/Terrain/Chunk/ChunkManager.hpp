#pragma once

#include <vector>
#include <memory>
#include "../../../Entities/Block/RawBlock.h"
#include "../../../Entities/Block/BlockTypes.h"
#include "Chunk.hpp"
#include "../../../Entities/Light/LightManager.hpp"

class ChunkManager
{
    robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> chunksHash;

public:
    void renderChunks(Shader& shader);

    void addChunk(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>>& blocksHash, glm::ivec2 chunkPosition, int generatedHeight);

    void setChunkMeshFromQueue(std::vector<BlockType>& blockTypes, robin_hood::unordered_map<glm::ivec3, unsigned char>& lightsHash);
    void batchChunk(glm::ivec2 chunkPosition);

    void checkEmittersBatch(LightManager& lightManager);

    void checkIfBlockOutOfBounds(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>>& blocksHash, glm::ivec2 chunkPosition);
    void unloadChunk(glm::ivec2 chunkPosition);

    robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& getChunksHash() { return chunksHash; }
};