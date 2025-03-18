#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Floor
{
  public:
    unsigned int textureID;
    unsigned int floorVAO, floorVBO, floorEBO;
    // Shader floorShader;

    std::string texture = "";

    float floorPositions[18] = {
        -10.0f, 0.0f, -10.0f, 10.0f, 0.0f, -10.0f, 10.0f,  0.0f, 10.0f,

        -10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 10.0f,  -10.0f, 0.0f, 10.0f};

    float floorNormals[18] = {
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

    float floorTexCoords[12] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                              0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    Floor()
    {
        glGenVertexArrays(1, &floorVAO);
        glGenBuffers(1, &floorVBO);
        glGenBuffers(1, &floorEBO);

        glBindVertexArray(floorVAO);

        glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(floorPositions) + sizeof(floorTexCoords), NULL,
                     GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floorPositions),
                        floorPositions);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(floorPositions),
                        sizeof(floorTexCoords), floorTexCoords);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

  private:
    unsigned int loadTexture(std::string texture)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        int with, height, nrChannels;
        unsigned char *data = stbi_load(texture.c_str(), &with, &height,
                                        &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, with, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        // I need the tex params here
    }
};
