#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define SCR_WIDTH 800
#define SCR_HEIGHT 800

class Window
{
	GLFWwindow* window;
    int nbFrames = 0;

	float lastFrame = 0;
	float deltaTime = 0;
	float lastTIME = glfwGetTime();
	const int FPS = 60;
public:
	Window();
	~Window();

	GLFWwindow &getWindow() { return *window; }

	void tick_window();

	float fps_limit()
	{
		double lastTime = glfwGetTime();
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		//std::cout << 1 / deltaTime << " FPS" << std::endl;
		//std::cout << deltaTime * 1000 << " ms" << std::endl;

		// fps limiter
		while (glfwGetTime() < lastTIME + 1.0 / FPS) {}
		lastTIME += 1.0 / FPS;
		lastFrame = currentFrame;

		return deltaTime;
	}
};