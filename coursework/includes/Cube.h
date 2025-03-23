#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube
{
  public:
    unsigned int VAO, VBO, textureID;
    Shader cubeShader;

    // Vertices for each face of the cube (Position, Normal, and Texture
    // Coordinates)
    float vertices[288] = {
        // Front face (2 triangles)
        -5.0f, -5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, -5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // Bottom-right
        5.0f, 5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // Top-right

        -5.0f, -5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, 5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // Top-right
        -5.0f, 5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // Top-left

        // Back face (2 triangles)
        -5.0f, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // Bottom-right
        5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // Top-right

        -5.0f, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // Top-right
        -5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // Top-left

        // Left face (2 triangles)
        -5.0f, 5.0f, 5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Top-left
        -5.0f, 5.0f, -5.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-left
        -5.0f, -5.0f, -5.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Bottom-right

        -5.0f, -5.0f, -5.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
        -5.0f, -5.0f, 5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-right
        -5.0f, 5.0f, 5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Top-left

        // Right face (2 triangles)
        5.0f, 5.0f, 5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Top-left
        5.0f, 5.0f, -5.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-left
        5.0f, -5.0f, -5.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Bottom-right

        5.0f, -5.0f, -5.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
        5.0f, -5.0f, 5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-right
        5.0f, 5.0f, 5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Top-left

        // Bottom face (2 triangles)
        -5.0f, -5.0f, -5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, -5.0f, -5.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
        5.0f, -5.0f, 5.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // Top-right

        -5.0f, -5.0f, -5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, -5.0f, 5.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // Top-right
        -5.0f, -5.0f, 5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // Top-left

        // Top face (2 triangles)
        -5.0f, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
        5.0f, 5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // Top-right

        -5.0f, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
        5.0f, 5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // Top-right
        -5.0f, 5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f   // Top-left
    };

    Cube() : cubeShader("shaders/cube.vs", "shaders/cube.fs")
    {
        // Generate and bind VAO, VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        // Set up vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void *)0); // Positions
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void *)(3 * sizeof(float))); // Normals
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void *)(6 * sizeof(float))); // Texture Coordinates
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Load texture
        textureID = loadTexture("grass.jpg");
    }

    void use(glm::mat4 model, glm::mat4 view, glm::mat4 projection,
             glm::vec3 lightPos, glm::vec3 lightColor, glm::vec3 cameraPos)
    {
        cubeShader.use();
        cubeShader.setMat4("model", model);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        cubeShader.setVec3("light.color", lightColor);
        cubeShader.setVec3("light.position", lightPos);
        cubeShader.setVec3("cameraPos", cameraPos);

        // Bind texture
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        cubeShader.setInt("material.diffuse", 0);
        cubeShader.setFloat("material.shininess", 1032.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

  private:
    unsigned int loadTexture(std::string texturePath)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        int width, height, nrChannels;
        unsigned char *data =
            stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture: " << texturePath << std::endl;
            stbi_image_free(data);
        }

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        return textureID;
    }
};
