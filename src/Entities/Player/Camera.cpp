#include "Camera.hpp"

void Camera::ProcessKeyboard(int direction, float camSpeed)
{
    if (direction == FORWARD)
        position += front * camSpeed;
    if (direction == BACKWARD)
        position -= front * camSpeed;
    if (direction == LEFT)
        position -= right * camSpeed;
    if (direction == RIGHT)
        position += right * camSpeed;
    if (direction == DOWN)
        position += glm::normalize(glm::cross(front, right)) * camSpeed;
    if (direction == UP)
        position -= glm::normalize(glm::cross(front, right)) * camSpeed;
}

void Camera::ProcessMouseMovement(float xpos, float ypos, GLboolean constrainPitch)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    std::cout << xoffset << ", " << yoffset << std::endl;

    float sensitivity = .075f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(pitch));
    front.y = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, up));
}