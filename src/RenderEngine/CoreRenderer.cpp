#include "CoreRenderer.hpp"

#include <glm/glm.hpp>
#include "../Window/Window.hpp"

CoreRenderer::CoreRenderer()
{

}

void CoreRenderer::render_all(Camera& camera, Shader& shader, ChunkManager& manager, float timeLight, float width, float height)
{
    if (timeLight < 0.025)
    {
        timeLight = .025;
    }

    if (timeLight > 1)
    {
        timeLight = 1;
    }

    // camera/view transformation
    glm::mat4 view = camera.getViewMatrix();
    shader.setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), (float)width / (float)height, 0.1f, 200.0f);
    shader.setMat4("projection", projection);

    glClearColor(.27f * timeLight, .55f * timeLight, 1.0f * timeLight, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.setFloat("timeLight", timeLight);

    manager.renderChunks(shader);
}