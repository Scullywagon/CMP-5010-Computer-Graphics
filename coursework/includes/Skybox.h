#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Skybox
{
  public:
    unsigned int textureID;

    void makeBox()
    {
        glGenTextures(1, &textureID);
        glBindTextures(GL_TEXTURE_CUBEMAP, textureID);
    }

  private:
};
