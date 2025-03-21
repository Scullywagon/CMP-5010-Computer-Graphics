#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

class Light
{
  public:
    glm::vec3 position =
        glm::vec3(-500.0f, 400.0f, -700.0f); // Far away, diagonal position
    glm::vec3 color = glm::vec3(1.0f, 0.9f, 0.8f); // White light
    float intensity = 0.1f;                        // Intensity of the light

    Shader lightShader;

    float result;

    Light() : lightShader("shaders/light.vs", "shaders/light.fs")
    {
    }

    void use(glm::mat4 view, glm::mat4 projection)
    {
        lightShader.use();
        lightShader.setVec3("lightPos", position);
        lightShader.setVec3("lightColor", color);
        lightShader.setFloat("lightIntensity", intensity);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
    }

  private:
};
