#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

class Light
{
  public:
    glm::vec3 position =
        glm::vec3(-500.0f, 800.0f, -700.0f); // Far away, diagonal position
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f); // White light
    float intensity = 0.9f;                        // Intensity of the light

    Shader lightShader;

    float result;

    Light() : lightShader("shaders/light.vs", "shaders/light.fs")
    {
    }

    glm::vec3 getColor()
    {
        return color * intensity;
    }

    void use(glm::mat4 view, glm::mat4 projection)
    {
    }

  private:
};
