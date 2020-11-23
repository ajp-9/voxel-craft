#pragma once

#include <vector>
#include <string>

#include "StructureTypes.h"
#include "Structure.h"

class StructureLoader
{
	std::vector<Structure> structureTypes;

public:
	StructureLoader();

	Structure LoadStructure(unsigned char id, std::string name, bool natural);

	std::vector<Structure> getStructureTypes() { return structureTypes; }
};