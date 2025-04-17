#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
  public:
    // mesh Data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 32.0f;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
         vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    // render the mesh
    // check if empty (wont be)
    // only doing diffuse so grab diffuse texture and bind
    // actve texture stuff etc 
    // bind the spec col and shininess
    // bind vertex array
    void Draw(Shader &shader, bool depthOnly = false)
    {
        bool useTextures = !textures.empty();
        shader.setBool("isTextured", useTextures);

        std::string materialName =
            (textures.size() > 1) ? "material2" : "material";

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            const std::string &type = textures[i].type;
            GLuint texID = textures[i].id;

            if (type == "texture_diffuse")
            {
                shader.setInt(materialName + ".diffuse", i);
                shader.setFloat(materialName + ".shininess", 3.0f);
            }
            else if (type == "texture_normal")
            {
                shader.setInt(materialName + ".normal", i);
                shader.setFloat(materialName + ".shininess", 4.0f);
            }

            glBindTexture(GL_TEXTURE_2D, texID);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                       GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        if (!depthOnly)
        {
            glActiveTexture(GL_TEXTURE0);
            shader.setBool("isTextured", false);
        }
    }

    void drawDepth(Shader &shader)
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                       GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void drawInstanced(Shader &shader, int instanceCount,
                       unsigned int instanceVBO)
    {
        shader.setBool("isInstanced", true);
        bool useTextures = !textures.empty();
        shader.setBool("isTextured", useTextures);

        std::string materialName =
            (textures.size() > 1) ? "material2" : "material";

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            const std::string &type = textures[i].type;
            GLuint texID = textures[i].id;

            if (type == "texture_diffuse")
            {
                shader.setInt(materialName + ".diffuse", i);
                shader.setFloat(materialName + ".shininess", 3.0f);
            }
            else if (type == "texture_normal")
            {
                shader.setInt(materialName + ".normal", i);
                shader.setFloat(materialName + ".shininess", 4.0f);
            }

            glBindTexture(GL_TEXTURE_2D, texID);
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        for (GLuint i = 0; i < 4; ++i)
        {
            glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE,
                                  sizeof(glm::mat4),
                                  (void *)(sizeof(glm::vec4) * i));
            glEnableVertexAttribArray(3 + i);
            glVertexAttribDivisor(3 + i,
                                  1); // This makes it an instance attribute
        }

        glDrawElementsInstanced(GL_TRIANGLES,
                                static_cast<unsigned int>(indices.size()),
                                GL_UNSIGNED_INT, 0, instanceCount);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        shader.setBool("isTextured", false);
        shader.setBool("isInstanced", false);
    }

  private:
    // render data
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int), &indices[0],
                     GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, TexCoords));
        glBindVertexArray(0);
    }
};
#endif
