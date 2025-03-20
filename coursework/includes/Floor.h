#include "Shader.h"
#include "stb_image.h"

class Floor
{
  public:
    unsigned int textureID;
    unsigned int floorVAO, floorVBO, floorEBO;
    Shader floorShader;

    std::string texture = "";

    float textureScale = 1000.0f;
    // Large square floor extending to the horizon
    float vertices[20] = {
        // Positions            // Texture Coordinates (repeat 50x)
        -2500.0f, 0.0f, -2500.0f, 0.0f,     0.0f,     // Bottom-left
        2500.0f,  0.0f, -2500.0f, textureScale, 0.0f,     // Bottom-right
        2500.0f,  0.0f, 2500.0f,  textureScale, textureScale, // Top-right
        -2500.0f, 0.0f, 2500.0f,  0.0f, textureScale  // Top-left
    };

    unsigned int indices[6] = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    Floor() : floorShader("shaders/floor.vs", "shaders/floor.fs")
    {
        textureID = loadTexture("grass.jpg");
        glGenVertexArrays(1, &floorVAO);
        glGenBuffers(1, &floorVBO);
        glGenBuffers(1, &floorEBO);

        glBindVertexArray(floorVAO);
        glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void use(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        floorShader.use();
        floorShader.setMat4("model", model);
        floorShader.setMat4("view", view);
        floorShader.setMat4("projection", projection);

        glBindVertexArray(floorVAO);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

  private:
    unsigned int loadTexture(std::string texture)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        int with, height, nrChannels;
        unsigned char *data =
            stbi_load(texture.c_str(), &with, &height, &nrChannels, 0);
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
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);

        return textureID;
    }
};
