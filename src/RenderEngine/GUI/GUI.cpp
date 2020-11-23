#include "GUI.hpp"

GUI::GUI()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

}

void GUI::draw(Shader& shader)
{
	
}