#ifndef SKYBOX_H
#define SKYBOX_H

#include "Shader.h"
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
    unsigned int texture1ID;
    unsigned int texture2ID;
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    Shader skyboxShader;
    float blendFactor = 0.5f;

    std::string textures1[8] = {"assets/skybox/right.jpg", "assets/skybox/left.jpg",
                                "assets/skybox/top.jpg",   "assets/skybox/bottom.jpg",
                                "assets/skybox/front.jpg", "assets/skybox/back.jpg"};

    float skyboxVertices[24] = {
        -1000.0f, 1000.0f,  -1000.0f, -1000.0f, -1000.0f, -1000.0f,
        1000.0f,  -1000.0f, -1000.0f, 1000.0f,  1000.0f,  -1000.0f,
        -1000.0f, 1000.0f,  1000.0f,  -1000.0f, -1000.0f, 1000.0f,
        1000.0f,  -1000.0f, 1000.0f,  1000.0f,  1000.0f,  1000.0f};

    unsigned int skyboxIndices[72] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
                                      0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3,
                                      0, 3, 7, 7, 4, 0, 1, 2, 6, 6, 5, 1};

    Skybox() : skyboxShader("shaders/skybox.vs", "shaders/skybox.fs")
    {
        for (float f : skyboxVertices)
        {
            f *= 100.0f;
            f -= 50000.0f;
        }
        texture1ID = loadCubemap(textures1);
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

    void render(glm::mat4 view, glm::mat4 projection)
    {
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(view));
        skyboxShader.setMat4("projection", projection);
        skyboxShader.setMat4("view", view);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture1ID);
        skyboxShader.setInt("skybox1", 0);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glBindVertexArray(0);
        // glUseProgram(0);
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

#endif
