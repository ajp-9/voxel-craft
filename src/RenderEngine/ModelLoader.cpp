#include "ModelLoader.hpp"

#include <iostream>

ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

Model ModelLoader::loadToVAO(float positions[], float textureCoords[], unsigned int indices[], int sizeofPos, int sizeofInd)
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO = VBOBinder(positions, textureCoords, sizeofPos);
	//unsigned int EBO = indiciesBinder(indices, sizeofInd);

	//Texture texture(textureName);
	glBindVertexArray(0);

	return Model(VAO, sizeofPos);
}

unsigned int ModelLoader::indiciesBinder(unsigned int indices[], int sizeofInd)
{
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sizeofInd, indices, GL_STATIC_DRAW);

	return EBO;
}

unsigned int ModelLoader::VBOBinder(float positions[], float textureCoords[], int sizeofPos)
{
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	std::vector<float> data;

	int posCount = 0;
	int texCount = 0;
	
	for (int i = 0; i < sizeofPos / 3; i++)
	{
		
		data.push_back(positions[posCount]);
		posCount++;
		data.push_back(positions[posCount]);
		posCount++;
		data.push_back(positions[posCount]);
		posCount++;
		data.push_back(textureCoords[texCount]);
		texCount++;
		data.push_back(textureCoords[texCount]);
		texCount++;
	}

	/*for (int i = 0; i < data.size(); i++)
	{
		std::cout << data[i] << "\n";
	}*/
	
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete(textureCoords); // ah yes, you have found the problem

	return VBO;
}