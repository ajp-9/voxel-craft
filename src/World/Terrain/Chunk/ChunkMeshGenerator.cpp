#include "ChunkMeshGenerator.hpp"

#include <thread>
#include <future>
#include "../../../Entities/Block/RawBlock.h"
#include "../../../Entities/Block/BlockTypes.h"

std::shared_ptr<ChunkMesh> ChunkMeshGenerator::generateChunkMesh(std::shared_ptr<robin_hood::unordered_map<glm::ivec3, unsigned char>>& blocksHash,
                                                                 std::vector<BlockType>& blockTypes,
                                                                 glm::ivec2 chunkPosition, robin_hood::unordered_map<glm::ivec2, std::shared_ptr<Chunk>>& chunksHash,
                                                                 robin_hood::unordered_map<glm::ivec3, unsigned char>& lightsHash,
                                                                 int generatedHeight)
{
    ///-------------------------------///-Putting each touching chunk blocksHash into storage so we dont have to go thru the long process of getting them-///

    robin_hood::unordered_map<glm::ivec3, unsigned char> frontChunkHash;
    robin_hood::unordered_map<glm::ivec3, unsigned char> front_rightChunkHash;
    robin_hood::unordered_map<glm::ivec3, unsigned char> backChunkHash;
    robin_hood::unordered_map<glm::ivec3, unsigned char> front_leftChunkHash;

    if (chunksHash.find(glm::ivec2(chunkPosition.x, chunkPosition.y - 1)) != chunksHash.end()) // checks to see if chunk exists or not
    {
        frontChunkHash = *chunksHash.at(glm::ivec2(chunkPosition.x, chunkPosition.y - 1))->getBlocksHash().get();
    }

    if (chunksHash.find(glm::ivec2(chunkPosition.x + 1, chunkPosition.y)) != chunksHash.end())
    {
        front_rightChunkHash = *chunksHash[glm::ivec2(chunkPosition.x + 1, chunkPosition.y)]->getBlocksHash().get();
    }

    if (chunksHash.find(glm::ivec2(chunkPosition.x, chunkPosition.y + 1)) != chunksHash.end())
    {
        backChunkHash = *chunksHash[glm::ivec2(chunkPosition.x, chunkPosition.y + 1)]->getBlocksHash().get();
    }

    if (chunksHash.find(glm::ivec2(chunkPosition.x - 1, chunkPosition.y)) != chunksHash.end())
    {
        front_leftChunkHash = *chunksHash[glm::ivec2(chunkPosition.x - 1, chunkPosition.y)]->getBlocksHash().get();
    }

    ///-------------------------------/// This is what I call- a *nightmare*
    std::vector<float> dataBuffer;
    std::vector<unsigned int> indices;
    unsigned int vertexCount = 0;
    int indicesRound = 0;
    //std::vector<std::future<void>> futures; // holding std::asyncs
    auto blocksHashStack = *blocksHash;

    for (auto& block : blocksHashStack) // first is coords, second is block/chunk
    {
        if (block.first.z >= generatedHeight)
        {
            if (block.second == block::rose)
            {
                addFlowerToMesh(block.first, block.second, blockTypes, lightsHash, vertexCount, indicesRound, dataBuffer, indices);
            }
            else if (block.second == block::torch)
            {
                addTorchToMesh(block.first, block.second, blockTypes, vertexCount, indicesRound, dataBuffer, indices);
            }
            else
            {
                addBlockToMesh(block.first, block.second, blockTypes, blocksHashStack, lightsHash, chunkPosition, frontChunkHash, front_rightChunkHash, backChunkHash, front_leftChunkHash,
                    vertexCount, indicesRound, dataBuffer, indices);
            }
        }
    }
    //std::cout << indices.size() << std::endl;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;

    //for (auto& b : dataBuffer)
    //{

    //    std::cout << b << std::endl;
    //}

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, dataBuffer.size() * sizeof(float), dataBuffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ------------------------ //

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // ------------------------ //

    glBindVertexArray(0);

    return std::make_shared<ChunkMesh>(VAO, VBO, EBO, vertexCount, chunkPosition);
}

void ChunkMeshGenerator::addFlowerToMesh(glm::ivec3 first, unsigned char second, std::vector<BlockType>& blockTypes, robin_hood::unordered_map<glm::ivec3, unsigned char> lightsHash, unsigned int& vertexCount, int& indicesRound, std::vector<float>& dataBuffer, std::vector<unsigned int>& indices)
{
    std::vector<float> tempBuff(blockTypes[second].packagedBufferData.begin(), blockTypes[second].packagedBufferData.end());

    int vt = 0; // counts the current vertex/texture coordinate (if uv then skips)
    for (int i = 0; i < (tempBuff.size() / 6); i++) //move vertices according to their block's position - TRANSFORMATION to world coords
    {
        tempBuff[vt] += first.x;
        tempBuff[vt + 1] += first.y;
        tempBuff[vt + 2] += first.z;
        
        vt = vt + 6;
    }

    vertexCount = vertexCount + (tempBuff.size() * .5); //add new vertices to vertex count
    for (int i = 0; i < 4; i++) // 6 (amount of floats per vtx) * 4 (vtxs per face)
    {
        if (i < 2)
        {
            std::vector<unsigned int> indiTemp = { 0, 1, 2, 2, 3, 0 };
            for (int d = 0; d < indiTemp.size(); d++)
            {
                indiTemp[d] = indiTemp[d] + (4 * indicesRound);
            }
            indicesRound++;
            indices.insert(indices.end(), indiTemp.begin(), indiTemp.end());
        }
        else
        {
            std::vector<unsigned int> indiTemp = { 0, 3, 2, 2, 1, 0 };
            for (int d = 0; d < indiTemp.size(); d++)
            {
                indiTemp[d] = indiTemp[d] + (4 * indicesRound);
            }
            indicesRound++;
            indices.insert(indices.end(), indiTemp.begin(), indiTemp.end());
        }
    }

    /*for (auto& b : tempBuff)
    {
        std::cout << b << std::endl;
    }*/

    dataBuffer.insert(dataBuffer.end(), tempBuff.begin(), tempBuff.end()); //put temp vertices into vbo buffer
}

void ChunkMeshGenerator::addTorchToMesh(glm::ivec3 first, unsigned char second, std::vector<BlockType>& blockTypes,
                                        unsigned int& vertexCount, int& indicesRound, std::vector<float>& dataBuffer,
                                        std::vector<unsigned int>& indices)
{
    std::vector<float> tempBuff(blockTypes[second].packagedBufferData.begin(), blockTypes[second].packagedBufferData.end());

    int vt = 0; // counts the current vertex/texture coordinate (if uv then skips)
    for (int i = 0; i < (tempBuff.size() / 6); i++) //move vertices according to their block's position - TRANSFORMATION to world coords
    {
        tempBuff[vt] += first.x;
        tempBuff[vt + 1] += first.y;
        tempBuff[vt + 2] += first.z;

        vt = vt + 6;
    }

    vertexCount = vertexCount + (tempBuff.size() * .5); //add new vertices to vertex count
    for (int i = 0; i < tempBuff.size() / 24; i++) // 6 (amount of floats per vtx) * 4 (vtxs per face)
    {
        std::vector<unsigned int> indiTemp = { 0, 1, 2, 2, 3, 0 };
        for (int d = 0; d < indiTemp.size(); d++)
        {
            indiTemp[d] = indiTemp[d] + (4 * indicesRound);
        }
        indicesRound++;
        indices.insert(indices.end(), indiTemp.begin(), indiTemp.end());
    }

    dataBuffer.insert(dataBuffer.end(), tempBuff.begin(), tempBuff.end()); //put temp vertices into vbo buffer
}

void ChunkMeshGenerator::addBlockToMesh(glm::ivec3 first, unsigned char second, std::vector<BlockType>& blockTypes,
    robin_hood::unordered_map<glm::ivec3, unsigned char>& blocksHash,
    robin_hood::unordered_map<glm::ivec3, unsigned char>& lightsHash,
    glm::ivec2& chunkPosition,
    robin_hood::unordered_map<glm::ivec3, unsigned char>& frontChunkHash,
    robin_hood::unordered_map<glm::ivec3, unsigned char>& front_rightChunkHash,
    robin_hood::unordered_map<glm::ivec3, unsigned char>& backChunkHash,
    robin_hood::unordered_map<glm::ivec3, unsigned char>& front_leftChunkHash,
    unsigned int& vertexCount, int& indicesRound, std::vector<float>& dataBuffer,
    std::vector<unsigned int>& indices)
{

    std::vector<float> tempBuff(blockTypes[second].packagedBufferData.begin(), blockTypes[second].packagedBufferData.end());
    bool frontFail = false, front_rightFail = false, backFail = false, front_leftFail = false, topFail = false, bottomFail = false;

    unsigned int frontBright = 80, front_rightBright = 70, backBright = 80, front_leftBright = 70, topBright = 100, bottomBright = 50; // set to 0 so quick as if bool

    unsigned int frontLight = 0, front_rightLight = 0, backLight = 0, front_leftLight = 0, topLight = 0, bottomLight = 0; // set to 0 so quick as if bool

    if (blocksHash.find(glm::ivec3(first.x, first.y - 1, first.z)) != blocksHash.end())
    {
        if (blocksHash[glm::ivec3(first.x, first.y - 1, first.z)] != block::rose && blocksHash[glm::ivec3(first.x, first.y - 1, first.z)] != block::torch)
            frontFail = true;
        else
            goto frontElse;
    }
    else
    {
    frontElse:
        if (lightsHash.find(glm::ivec3(first.x, first.y - 1, first.z)) != lightsHash.end())
        {
            frontLight = lightsHash[glm::ivec3(first.x, first.y - 1, first.z)];
        }
    }

    if (blocksHash.find(glm::ivec3(first.x + 1, first.y, first.z)) != blocksHash.end())
    {
        if (blocksHash[glm::ivec3(first.x + 1, first.y, first.z)] != block::rose && blocksHash[glm::ivec3(first.x + 1, first.y, first.z)] != block::torch)
            front_rightFail = true;
        else
            goto front_rightElse;
    }
    else
    {
    front_rightElse:
        if (lightsHash.find(glm::ivec3(first.x + 1, first.y, first.z)) != lightsHash.end())
        {
            front_rightLight = lightsHash.at(glm::ivec3(first.x + 1, first.y, first.z));
        }
    }

    if (blocksHash.find(glm::ivec3(first.x, first.y + 1, first.z)) != blocksHash.end())
    {
        if (blocksHash[glm::ivec3(first.x, first.y + 1, first.z)] != block::rose && blocksHash[glm::ivec3(first.x, first.y + 1, first.z)] != block::torch)
            backFail = true;
        else
            goto backElse;
    }
    else
    {
    backElse:
        if (lightsHash.find(glm::ivec3(first.x, first.y + 1, first.z)) != lightsHash.end())
        {
            backLight = lightsHash[glm::ivec3(first.x, first.y + 1, first.z)];
        }
    }

    if (blocksHash.find(glm::ivec3(first.x - 1, first.y, first.z)) != blocksHash.end())
    {
        if (blocksHash[glm::ivec3(first.x - 1, first.y, first.z)] != block::rose && blocksHash[glm::ivec3(first.x - 1, first.y, first.z)] != block::torch)
            front_leftFail = true;
        else
            goto front_leftElse;
    }
    else
    {
    front_leftElse:
        if (lightsHash.find(glm::ivec3(first.x - 1, first.y, first.z)) != lightsHash.end())
        {
            front_leftLight = lightsHash.at(glm::ivec3(first.x - 1, first.y, first.z));
        }
    }

    if (blocksHash.find(glm::ivec3(first.x, first.y, first.z + 1)) != blocksHash.end())
    {
        if (blocksHash[glm::ivec3(first.x, first.y, first.z + 1)] != block::rose && blocksHash[glm::ivec3(first.x, first.y, first.z + 1)] != block::torch)
            topFail = true;
        else
            goto topElse;
    }
    else
    {
    topElse:
        if (lightsHash.find(glm::ivec3(first.x, first.y, first.z + 1)) != lightsHash.end())
        {
            topLight = lightsHash[glm::ivec3(first.x, first.y, first.z + 1)];
        }
    }

    if (blocksHash.find(glm::ivec3(first.x, first.y, first.z - 1)) != blocksHash.end())
    {
        if (blocksHash[glm::ivec3(first.x, first.y, first.z - 1)] != block::rose && blocksHash[glm::ivec3(first.x, first.y, first.z - 1)] != block::torch)
            bottomFail = true;
        else
            goto bottomElse;
    }
    else
    {
    bottomElse:
        if (lightsHash.find(glm::ivec3(first.x, first.y, first.z - 1)) != lightsHash.end())
        {
            bottomLight = lightsHash[glm::ivec3(first.x, first.y, first.z - 1)];
        }
    }

    bool frontFail_2 = false, front_rightFail_2 = false, backFail_2 = false, front_leftFail_2 = false;

    // another round for only those which have faces to compare with the chunks
    
    if (!frontFail)
    {
        if (frontChunkHash.find(glm::ivec3(first.x, first.y - 1, first.z)) != frontChunkHash.end())
        {
            if (frontChunkHash[glm::ivec3(first.x, first.y - 1, first.z)] != block::rose && frontChunkHash[glm::ivec3(first.x, first.y - 1, first.z)] != block::torch)
                frontFail_2 = true;
            else
                goto frontElse2;
        }
        else
        {
        frontElse2:
            if (lightsHash.find(glm::ivec3(first.x, first.y - 1, first.z)) != lightsHash.end())
            {
                frontLight = lightsHash[glm::ivec3(first.x, first.y - 1, first.z)];
            }
        }
    }

    if (!front_rightFail)
    {
        if (front_rightChunkHash.find(glm::ivec3(first.x + 1, first.y, first.z)) != front_rightChunkHash.end())
        {
            if (front_rightChunkHash[glm::ivec3(first.x + 1, first.y, first.z)] != block::rose && front_rightChunkHash[glm::ivec3(first.x + 1, first.y, first.z)] != block::torch)
                front_rightFail_2 = true;
            else
                goto front_rightElse2;
        }
        else
        {
        front_rightElse2:
            if (lightsHash.find(glm::ivec3(first.x + 1, first.y, first.z)) != lightsHash.end())
            {
                front_rightLight = lightsHash[glm::ivec3(first.x + 1, first.y, first.z)];
            }
        }
    }

    if (!backFail)
    {
        if (backChunkHash.find(glm::ivec3(first.x, first.y + 1, first.z)) != backChunkHash.end())
        {
            if (backChunkHash[glm::ivec3(first.x, first.y + 1, first.z)] != block::rose && backChunkHash[glm::ivec3(first.x, first.y + 1, first.z)] != block::torch)
                backFail_2 = true;
            else
                goto backElse2;
        }
        else
        {
        backElse2:
            if (lightsHash.find(glm::ivec3(first.x, first.y + 1, first.z)) != lightsHash.end())
            {
                backLight = lightsHash[glm::ivec3(first.x, first.y + 1, first.z)];
            }
        }
    }

    if (!front_leftFail)
    {
        if (front_leftChunkHash.find(glm::ivec3(first.x - 1, first.y, first.z)) != front_leftChunkHash.end())
        {
            if (front_leftChunkHash[glm::ivec3(first.x - 1, first.y, first.z)] != block::rose && front_leftChunkHash[glm::ivec3(first.x - 1, first.y, first.z)] != block::torch)
                front_leftFail_2 = true;
            else
                goto front_leftElse2;
        }
        else
        {
        front_leftElse2:
            if (lightsHash.find(glm::ivec3(first.x - 1, first.y, first.z)) != lightsHash.end())
            {
                front_leftLight = lightsHash[glm::ivec3(first.x - 1, first.y, first.z)];
            }
        }
    }

    if (frontFail)
        frontFail_2 = true;

    if (front_rightFail)
        front_rightFail_2 = true;

    if (backFail)
        backFail_2 = true;

    if (front_leftFail)
        front_leftFail_2 = true;

    std::vector<float> bufferTempFinished;
    bufferTempFinished.reserve(128);
    std::vector<float> brightnessOfSides;
    brightnessOfSides.reserve(128);

    if (!frontFail_2)
    {
        bufferTempFinished.insert(bufferTempFinished.end(), tempBuff.begin(), tempBuff.begin() + 24);

        int finalLight = (frontBright * 1000) + frontLight; // bright then light

        brightnessOfSides.insert(brightnessOfSides.end(), { (float)finalLight } );
    }

    if (!front_rightFail_2)
    {
        bufferTempFinished.insert(bufferTempFinished.end(), tempBuff.begin() + 24, tempBuff.begin() + 48);

        int finalLight = (front_rightBright * 1000) + front_rightLight; // bright then light

        brightnessOfSides.insert(brightnessOfSides.end(), { (float)finalLight } );

    }

    if (!backFail_2)
    { 
        bufferTempFinished.insert(bufferTempFinished.end(), tempBuff.begin() + 48, tempBuff.begin() + 72);

        int finalLight = (backBright * 1000) + backLight; // bright then light

        brightnessOfSides.insert(brightnessOfSides.end(), { (float)finalLight } );

    }

    if (!front_leftFail_2)
    { 
        bufferTempFinished.insert(bufferTempFinished.end(), tempBuff.begin() + 72, tempBuff.begin() + 96);

        int finalLight = (front_leftBright * 1000) + front_leftLight; // bright then light

        brightnessOfSides.insert(brightnessOfSides.end(), { (float)finalLight } );

    }

    if (!topFail) // b/c not testing for chunk
    {
        bufferTempFinished.insert(bufferTempFinished.end(), tempBuff.begin() + 96, tempBuff.begin() + 120);

        int finalLight = (topBright * 1000) + topLight; // bright then light

        brightnessOfSides.insert(brightnessOfSides.end(), { (float)finalLight } );

    }

    if (!bottomFail) // b/c not testing for chunk
    {
        bufferTempFinished.insert(bufferTempFinished.end(), tempBuff.begin() + 120, tempBuff.begin() + 144);

        int finalLight = (bottomBright * 1000) + bottomLight; // bright then light

        brightnessOfSides.insert(brightnessOfSides.end(), { (float)finalLight } );
    }
    
    int vt = 0; // counts the current vertex/texture coordinate (if uv then skips)
    float brightCount = 0;
    for (int i = 0; i < (bufferTempFinished.size() / 6); i++) //move vertices according to their block's position - TRANSFORMATION to world coords
    {
        bufferTempFinished[vt] += first.x;
        bufferTempFinished[vt + 1] += first.y;
        bufferTempFinished[vt + 2] += first.z;

        bufferTempFinished[vt + 5] = brightnessOfSides[brightCount];
        brightCount += .25; // so for at a time

        vt = vt + 6;
    }

    vertexCount = vertexCount + (bufferTempFinished.size() * .5); //add new vertices to vertex count

    for (int i = 0; i < (bufferTempFinished.size() / 24); i++) // 6 (amount of floats per vtx) * 4 (vtxs per face)
    {
        std::vector<unsigned int> indiTemp = { 0, 1, 2, 2, 3, 0 };
        for (auto& i : indiTemp)
        {
            i = i + (4 * indicesRound);
        }
        indicesRound++;
        indices.insert(indices.end(), indiTemp.begin(), indiTemp.end());
    }

    dataBuffer.insert(dataBuffer.end(), bufferTempFinished.begin(), bufferTempFinished.end()); //put temp vertices into vbo buffer
}