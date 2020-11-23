#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp> //

#include "Window/Window.hpp"
#include "RenderEngine/Shader/Shader.h"
#include "RenderEngine/Textures/Texture.hpp"
#include "Entities/Player/Player.hpp"
#include "RenderEngine/CoreRenderer.hpp"

#include "World/BlockManager.hpp"
#include "World/Terrain/WorldGenerator/TerrainGenerator.hpp"

#include "World/Terrain/Chunk/ChunkMeshGenerator.hpp"

Player player(glm::vec3(0, 0, 34)); // put first for camera callbacks

float deltaTime = 0.0f;	// time between current frame and last frame
float scr_width = 800, scr_height = 800;
float last_width = 0, last_height = 0;

#include "Entities/Player/GLFWcallbacks.h"

int main()
{
	CoreRenderer core;

	Window window;

	BlockManager blockManager;
	blockManager.loadBlockTypes();

	ChunkManager chunkManager;
	player.setManager(chunkManager);

	TerrainGenerator generator;

	generator.generate(chunkManager, blockManager.getBlockTypes(), player.getPosition());

	std::vector<int> test = { 1, 2, 3, 4, 5};
	test.erase(test.begin() + 1);

	for (auto te : test)
	{
		//std::cout << te << std::endl;
	}

	Shader shader("Shaders/Shader.vs", "Shaders/Shader.fs");

	Texture texture("res/textures/atlas.png");
	texture.bind();
	shader.bind();

	int count = 0;
	while (!glfwWindowShouldClose(&window.getWindow()))
	{
		processInput(&window.getWindow());

		generator.generate(chunkManager, blockManager.getBlockTypes(), player.getPosition());

		core.render_all(player, shader, chunkManager, (std::cos(glfwGetTime() * .01) * .9) + .6, scr_width, scr_height);

		//std::cout << std::round(player.getPosition().x) << ", " << std::round(player.getPosition().y) << ", " << std::round(player.getPosition().z) << std::endl;
 
		deltaTime = window.fps_limit();
		window.tick_window();
	}

	window.~Window();
	return 0;
}