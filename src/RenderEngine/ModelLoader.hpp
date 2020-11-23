#pragma once

#include <vector>
#include "../Entities/Model.h"

struct ModelLoader
{
	ModelLoader();
	~ModelLoader();

	Model static loadToVAO(float positions[], float textureCoords[], unsigned int indices[], int sizeofPos, int sizeofInd);
	unsigned int static indiciesBinder(unsigned int indices[], int sizeofInd);

private:
	std::vector<unsigned int> VAOs;
	std::vector<unsigned int> VBOs;
	std::vector<unsigned int> EBOs;

	unsigned int static VBOBinder(float positions[], float textureCoords[], int sizeofPos);
};