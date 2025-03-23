#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

struct vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
  public:
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    texture tex;

    Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices,
         texture tex)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->tex = tex;

        setupMesh();
    }

    void draw(Shader &shader)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex.id);
        shader.setInt("material.diffuse", 0);
        shader.setFloat("material.shininess", 32.0f);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

  private:
    unsigned int VBO, VAO, EBO;

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex),
                     &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int), &indices[0],
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                              (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                              (void *)offsetof(vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                              (void *)offsetof(vertex, TexCoords));
    }
};

#endif
