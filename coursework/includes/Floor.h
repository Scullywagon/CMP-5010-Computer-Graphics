#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"

class Floor
{
  public:
    unsigned int textureID;
    unsigned int VAO, positionVBO, normalVBO, texCoordVBO, EBO;
    Shader floorShader;

    std::string texture = "";

    float textureScale = 1000.0f;
    // Large square floor extending to the horizon
    float positions[12] = {
        -2500.0f, 0.0f, -2500.0f, // Bottom-left
        2500.0f,  0.0f, -2500.0f, // Bottom-right
        2500.0f,  0.0f, 2500.0f,  // Top-right
        -2500.0f, 0.0f, 2500.0f   // Top-left
    };

    float normals[12] = {
        0.0f, 1.0f, 0.0f, // Normal for bottom-left
        0.0f, 1.0f, 0.0f, // Normal for bottom-right
        0.0f, 1.0f, 0.0f, // Normal for top-right
        0.0f, 1.0f, 0.0f  // Normal for top-left
    };

    float texCoords[12] = {
        0.0f,         0.0f,         // Bottom-left
        textureScale, 0.0f,         // Bottom-right
        textureScale, textureScale, // Top-right
        0.0f,         textureScale  // Top-left
    };

    unsigned int dd[6] = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    std::vector<vertex> vertices = {
        {{-2500.0f, 10.0f, -2500.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{2500.0f, 10.0f, -2500.0f}, {0.0f, 1.0f, 0.0f}, {textureScale, 0.0f}},
        {{2500.0f, 10.0f, 2500.0f}, {0.0f, 1.0f, 0.0f}, {textureScale, textureScale}},
        {{-2500.0f, 10.0f, 2500.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, textureScale}}
        };

    // Create a vector of indices
    std::vector<unsigned int> indices = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    Floor() : floorShader("shaders/cube.vs", "shaders/cube.fs")
    {
        textureID = loadTexture("grass.jpg");

    }

    void use(glm::mat4 model, glm::mat4 view, glm::mat4 projection,
             glm::vec3 lightPos, glm::vec3 lightColor, glm::vec3 cameraPos)
    {
        floorShader.use();
        floorShader.setMat4("model", model);
        floorShader.setMat4("view", view);
        floorShader.setMat4("projection", projection);
        floorShader.setVec3("light.position", lightPos);
        floorShader.setVec3("light.color", lightColor);
        floorShader.setVec3("cameraPos", cameraPos);

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        floorShader.setInt("material.diffuse", 0);
        // flloorShader.setVec3("material.specular")
        floorShader.setFloat("material.shininess", 32.0f);
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
