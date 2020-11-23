#include "Chunk.hpp"

Chunk::Chunk(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>> blocksHash, short unsigned int generatedHeight)
    : blocksHash(blocksHash), generatedHeight(generatedHeight) {}