#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Skybox
{
  public:
    unsigned int textureID;
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    Shader skyboxShader;

    std::string textures[8] = {"skybox/right.jpg", "skybox/left.jpg",
                               "skybox/top.jpg",   "skybox/bottom.jpg",
                               "skybox/front.jpg", "skybox/back.jpg"};

    float skyboxVertices[24] = {
        -1.0f, 1.0f,  -1.0f, // 0 - Top-left-front
        -1.0f, -1.0f, -1.0f, // 1 - Bottom-left-front
        1.0f,  -1.0f, -1.0f, // 2 - Bottom-right-front
        1.0f,  1.0f,  -1.0f, // 3 - Top-right-front
        -1.0f, 1.0f,  1.0f,  // 4 - Top-left-back
        -1.0f, -1.0f, 1.0f,  // 5 - Bottom-left-back
        1.0f,  -1.0f, 1.0f,  // 6 - Bottom-right-back
        1.0f,  1.0f,  1.0f   // 7 - Top-right-back
    };

    unsigned int skyboxIndices[72] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 1, 5, 5, 4, 0,
        // Right face
        3, 2, 6, 6, 7, 3,
        // Top face
        0, 3, 7, 7, 4, 0,
        // Bottom face
        1, 2, 6, 6, 5, 1};

    Skybox() : skyboxShader("shaders/skybox.vs", "shaders/skybox.fs")
    {
        textureID = loadCubemap(textures);
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glGenBuffers(1, &skyboxEBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices,
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices),
                     skyboxIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void use(glm::mat4 view, glm::mat4 projection)
    {
        glDepthMask(GL_FALSE);
        view = glm::mat4(glm::mat3(view));
        skyboxShader.setMat4("projection", projection);
        skyboxShader.setMat4("view", view);
        skyboxShader.use();

        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
    }

  private:
    unsigned int loadCubemap(std::string faces[])
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < 6; i++)
        {
            unsigned char *data =
                stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                             width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i]
                          << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                        GL_CLAMP_TO_EDGE);

        return textureID;
    }
};
