#pragma once

#include <glad/glad.h>

#include "../Shader/Shader.h"

class GUI
{
	unsigned int VAO;
	unsigned int VBO;

public:
	GUI();
	void draw(Shader& shader);
};