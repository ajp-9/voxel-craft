#pragma once

bool maximized = false;
float lastTime = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window)
{
    float camSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camSpeed = cam::FAST_SPEED * deltaTime;
    }
    else
    {
        camSpeed = cam::SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.ProcessKeyboard(FORWARD, camSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.ProcessKeyboard(BACKWARD, camSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.ProcessKeyboard(LEFT, camSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.ProcessKeyboard(RIGHT, camSpeed);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        player.ProcessKeyboard(DOWN, camSpeed);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        player.ProcessKeyboard(UP, camSpeed);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        player.setCurrentBlock(0);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        player.setCurrentBlock(1);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        player.setCurrentBlock(2);
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        player.setCurrentBlock(3);
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        player.setCurrentBlock(4);
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        player.setCurrentBlock(5);
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        player.setCurrentBlock(6);
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        player.setCurrentBlock(block::glass);
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        player.setCurrentBlock(block::rose);
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        player.setCurrentBlock(block::torch); 
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {   
        if (lastTime < glfwGetTime() - .5)
        {
            if (!maximized)
            {
                glfwMaximizeWindow(window);
                lastTime = glfwGetTime();
                maximized = true;
            }
            else
            {
                glfwSetWindowSize(window, 800, 800);
                lastTime = glfwGetTime();
                maximized = false;
            }
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    scr_width = width;
    scr_height = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    player.ProcessMouseMovement(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        player.breakBlock();
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        player.placeBlock();
    }
}