#ifndef LIGHT_H
#define LIGHT_H

#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "glm/gtc/quaternion.hpp"
#include <glm/gtc/type_ptr.hpp>

struct Sun
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 translation = glm::vec3(0.0f, -1.0f, 0.0f) * -1000.0f;

    Mesh *mesh;
    float size = 20.0f;
    float textureScale = 1.0f;
    std::vector<Texture> textures;
    std::vector<Vertex> vertices = {
        {{-size, 0.0f, -size}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{size, 0.0f, -size}, {0.0f, 1.0f, 0.0f}, {textureScale, 0.0f}},
        {{size, 0.0f, size}, {0.0f, 1.0f, 0.0f}, {textureScale, textureScale}},
        {{-size, 0.0f, size}, {0.0f, 1.0f, 0.0f}, {0.0f, textureScale}}};
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    Sun()
    {
        Texture tex;
        unsigned int textureID = TextureFromFile("moon.png", "assets/", false);
        tex.id = textureID;
        tex.type = "texture_diffuse";
        tex.path = "assets/terrain/FloorTile/Terreno_TileMat_2.png";

        textures.push_back(tex);

        mesh = new Mesh(vertices, indices, textures);
        direction = glm::vec3(0.0f, -1.0f, 0.0f);
        ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
        specular = glm::vec3(0.4f, 0.4f, 0.4f);

        modelMatrix = glm::translate(modelMatrix, translation);
    }

    float speed = 10.0f; // Speed of rotation
    void move(float deltaTime)
    {
        float angle = deltaTime * speed;

        // Rotate the direction around the Y-axis (the center of the orbit)
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle),
                                         glm::vec3(0.0f, 0.0f, 1.0f));
        direction = glm::vec3(rotation * glm::vec4(direction, 1.0f));

        // Orbit the object around the origin by updating the model matrix
        modelMatrix = glm::translate(
            modelMatrix, -translation); // Move the object to the origin
        modelMatrix =
            glm::rotate(modelMatrix, glm::radians(angle),
                        glm::vec3(0.0f, 0.0f, 1.0f)); // Apply rotation
        modelMatrix = glm::translate(
            modelMatrix, translation); // Move the object back to its position
    }

    void draw(Shader &shader)
    {
        shader.setMat4("model", modelMatrix);
        mesh->Draw(shader, false);
    }
};

struct Light
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#endif
