#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"

class Cube
{
  public:
    unsigned int textureID;
    Shader cubeShader;

    Mesh *mesh;
    Texture tex;

    // Vertices for each face of the cube (Position, Normal, and Texture
    // Coordinates)
    float size = 10.0f;                             // Size of the cube
    glm::vec3 center = glm::vec3(0.0f, 5.0f, 0.0f); // Center of the cube

    // Define the normals for the cube faces (one for each face)
    std::vector<glm::vec3> normals = {
        glm::vec3(0.0f, 0.0f, -1.0f), // Front
        glm::vec3(0.0f, 0.0f, 1.0f),  // Back
        glm::vec3(-1.0f, 0.0f, 0.0f), // Left
        glm::vec3(1.0f, 0.0f, 0.0f),  // Right
        glm::vec3(0.0f, -1.0f, 0.0f), // Bottom
        glm::vec3(0.0f, 1.0f, 0.0f)   // Top
    };

    // Define the texture coordinates for the cube (each face has 4 corners)
    std::vector<glm::vec2> texCoords = {
        glm::vec2(0.0f, 0.0f), // Bottom-left
        glm::vec2(1.0f, 0.0f), // Bottom-right
        glm::vec2(1.0f, 1.0f), // Top-right
        glm::vec2(0.0f, 1.0f)  // Top-left
    };

    // Define the 12 triangles (2 per face)
    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Left face
        8, 9, 10, 8, 10, 11,
        // Right face
        12, 13, 14, 12, 14, 15,
        // Bottom face
        16, 17, 18, 16, 18, 19,
        // Top face
        20, 21, 22, 20, 22, 23};

    std::vector<Texture> textures;

    Cube() : cubeShader("shaders/cube.vs", "shaders/cube.fs")
    {
        textureID = loadTexture("grass.jpg");
        tex.id = textureID;
        tex.type = "texture_diffuse";
        tex.path = "grass.jpg";

        textures.push_back(tex);

        std::vector<Vertex> vertices = {
            // Front face (normal = -Z)
            {glm::vec3(center.x - size / 2, center.y - size / 2,
                       center.z - size / 2),
             normals[0], glm::vec2(0.0f, 0.0f)}, // Bottom-left-front
            {glm::vec3(center.x + size / 2, center.y - size / 2,
                       center.z - size / 2),
             normals[0], glm::vec2(1.0f, 0.0f)}, // Bottom-right-front
            {glm::vec3(center.x + size / 2, center.y + size / 2,
                       center.z - size / 2),
             normals[0], glm::vec2(1.0f, 1.0f)}, // Top-right-front
            {glm::vec3(center.x - size / 2, center.y + size / 2,
                       center.z - size / 2),
             normals[0], glm::vec2(0.0f, 1.0f)}, // Top-left-front

            // Back face (normal = +Z)
            {glm::vec3(center.x - size / 2, center.y - size / 2,
                       center.z + size / 2),
             normals[1], glm::vec2(0.0f, 0.0f)}, // Bottom-left-back
            {glm::vec3(center.x + size / 2, center.y - size / 2,
                       center.z + size / 2),
             normals[1], glm::vec2(1.0f, 0.0f)}, // Bottom-right-back
            {glm::vec3(center.x + size / 2, center.y + size / 2,
                       center.z + size / 2),
             normals[1], glm::vec2(1.0f, 1.0f)}, // Top-right-back
            {glm::vec3(center.x - size / 2, center.y + size / 2,
                       center.z + size / 2),
             normals[1], glm::vec2(0.0f, 1.0f)}, // Top-left-back

            // Left face (normal = -X)
            {glm::vec3(center.x - size / 2, center.y - size / 2,
                       center.z - size / 2),
             normals[2], glm::vec2(0.0f, 0.0f)}, // Bottom-left-left
            {glm::vec3(center.x - size / 2, center.y - size / 2,
                       center.z + size / 2),
             normals[2], glm::vec2(1.0f, 0.0f)}, // Bottom-right-left
            {glm::vec3(center.x - size / 2, center.y + size / 2,
                       center.z + size / 2),
             normals[2], glm::vec2(1.0f, 1.0f)}, // Top-right-left
            {glm::vec3(center.x - size / 2, center.y + size / 2,
                       center.z - size / 2),
             normals[2], glm::vec2(0.0f, 1.0f)}, // Top-left-left

            // Right face (normal = +X)
            {glm::vec3(center.x + size / 2, center.y - size / 2,
                       center.z - size / 2),
             normals[3], glm::vec2(0.0f, 0.0f)}, // Bottom-left-right
            {glm::vec3(center.x + size / 2, center.y - size / 2,
                       center.z + size / 2),
             normals[3], glm::vec2(1.0f, 0.0f)}, // Bottom-right-right
            {glm::vec3(center.x + size / 2, center.y + size / 2,
                       center.z + size / 2),
             normals[3], glm::vec2(1.0f, 1.0f)}, // Top-right-right
            {glm::vec3(center.x + size / 2, center.y + size / 2,
                       center.z - size / 2),
             normals[3], glm::vec2(0.0f, 1.0f)}, // Top-left-right

            // Bottom face (normal = -Y)
            {glm::vec3(center.x - size / 2, center.y - size / 2,
                       center.z - size / 2),
             normals[4], glm::vec2(0.0f, 0.0f)}, // Bottom-left-bottom
            {glm::vec3(center.x + size / 2, center.y - size / 2,
                       center.z - size / 2),
             normals[4], glm::vec2(1.0f, 0.0f)}, // Bottom-right-bottom
            {glm::vec3(center.x + size / 2, center.y - size / 2,
                       center.z + size / 2),
             normals[4], glm::vec2(1.0f, 1.0f)}, // Top-right-bottom
            {glm::vec3(center.x - size / 2, center.y - size / 2,
                       center.z + size / 2),
             normals[4], glm::vec2(0.0f, 1.0f)}, // Top-left-bottom

            // Top face (normal = +Y)
            {glm::vec3(center.x - size / 2, center.y + size / 2,
                       center.z - size / 2),
             normals[5], glm::vec2(0.0f, 0.0f)}, // Bottom-left-top
            {glm::vec3(center.x + size / 2, center.y + size / 2,
                       center.z - size / 2),
             normals[5], glm::vec2(1.0f, 0.0f)}, // Bottom-right-top
            {glm::vec3(center.x + size / 2, center.y + size / 2,
                       center.z + size / 2),
             normals[5], glm::vec2(1.0f, 1.0f)}, // Top-right-top
            {glm::vec3(center.x - size / 2, center.y + size / 2,
                       center.z + size / 2),
             normals[5], glm::vec2(0.0f, 1.0f)} // Top-left-top
        };
        mesh = new Mesh(vertices, indices, textures);
    }

    void use(Shader &shader)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set the shader uniforms
        mesh->Draw(cubeShader);
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
#endif
