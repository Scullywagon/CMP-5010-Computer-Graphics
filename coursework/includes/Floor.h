#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"

class Floor
{
  public:
    unsigned int textureID;
    Shader floorShader;
    Mesh *mesh;

    texture tex;

    float textureScale = 1000.0f;
    float floorSize = 2500.0f;
    // Large square floor extending to the horizon

    std::vector<vertex> vertices = {
        {{-floorSize, 0.0f, -floorSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{floorSize, 0.0f, -floorSize}, {0.0f, 1.0f, 0.0f}, {textureScale, 0.0f}},
        {{floorSize, 0.0f, floorSize}, {0.0f, 1.0f, 0.0f}, {textureScale, textureScale}},
        {{-floorSize, 0.0f, floorSize}, {0.0f, 1.0f, 0.0f}, {0.0f, textureScale}}
        };

    // Create a vector of indices
    std::vector<unsigned int> indices = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    Floor() : floorShader("shaders/cube.vs", "shaders/cube.fs")
    {
        textureID = loadTexture("grass.jpg");

        tex.id = textureID;
        tex.type = "texture_diffuse";
        tex.path = "grass.jpg";

        mesh = new Mesh(vertices, indices, tex);
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

        mesh->draw(floorShader);
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
