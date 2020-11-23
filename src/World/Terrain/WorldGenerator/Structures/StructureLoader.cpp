#include "StructureLoader.hpp"

#include <iostream>
#include <fstream>

StructureLoader::StructureLoader()
{
    structureTypes.push_back(LoadStructure(structure::tree, "tree", 1));
}

Structure StructureLoader::LoadStructure(unsigned char id, std::string name, bool natural)
{
    std::vector<std::pair<glm::ivec3, unsigned char>> blocks;

    std::string buffer;
    glm::ivec3 currentBlockPos;
    char ch;

    std::fstream fin;
    
    try
    {
        if (natural)
        {
            fin = std::fstream("res/structures/natural/" + name + ".str", std::fstream::in);
        }
        else
        {
            fin = std::fstream("res/structures/artificial/" + name + ".str", std::fstream::in);
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Error when opening file: " << e.what() << std::endl;
    }

    unsigned char currentBlockID = 0;

    bool comment = false;
    bool type = false;
    bool block = false;
    
    int currentXYZ = 0;

    int line = 1; // for catching errors, i know ur not perfect =p

    try
    {
        while (fin >> std::noskipws >> ch)
        {
            if (ch == '\n')
            {
                line++;
            }

            if (ch == '#') //checks for a comment
            {
                if (comment)
                {
                    comment = false;
                    continue;
                }
                else
                {
                    comment = true;
                    continue;
                }
            }

            if (!comment) // skips if comment
            {
                if (ch == '!')
                {
                    if (type)
                    {
                        currentBlockID = std::stoi(buffer);
                        buffer.clear();

                        type = false;
                    }
                    else
                    {
                        type = true;
                    }
                    continue;
                }

                if (ch == ';')
                {
                    if (block)
                    {
                        currentBlockPos.z = std::stoi(buffer);
                        blocks.push_back(std::make_pair(currentBlockPos, currentBlockID));

                        currentXYZ = 0;
                        buffer.clear();
                        block = false;
                    }
                    else
                    {
                        block = true;
                    }

                    continue;
                }

                // | bools so shit isnt added to buffer => broken
                // V

                if (type) // if type is active adds to buffer;
                {
                    buffer += ch;
                    continue;
                }

                if (block)
                {
                    if (ch == ',')
                    {
                        if (currentXYZ == 0) // puts first number
                        {
                            currentBlockPos.x = std::stoi(buffer);
                            currentXYZ++;
                            buffer.clear();
                            continue;
                        }

                        if (currentXYZ == 1)
                        {
                            currentBlockPos.y = std::stoi(buffer);
                            currentXYZ++;
                            buffer.clear();
                            continue;
                        }
                    }

                    buffer += ch;
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Error when reading structure file: " << e.what() << ". At line " << line << std::endl;
    }

    fin.close();
    
    return Structure(id, blocks);
}