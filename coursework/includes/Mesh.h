#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BoundingBox.h"
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
    // tangent
    // glm::vec3 Tangent;
    // bitangent
    //  glm::vec3 Bitangent;
    // bone indexes which will influence this vertex
    //   int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    //    float m_Weights[MAX_BONE_INFLUENCE];
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

    vector<BoundingBox *> boundingBoxes;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
         vector<Texture> textures, bool subBoxes)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and
        // its attribute pointers.
        setupMesh();

        if (!subBoxes)
        {
            return;
        }

        for (int i = 0; i < indices.size(); i += 3)
        {
            glm::vec3 p1 = vertices[indices[i]].Position;
            glm::vec3 p2 = vertices[indices[i + 1]].Position;
            glm::vec3 p3 = vertices[indices[i + 2]].Position;

            // Initialize the min and max points for the triangle
            glm::vec3 minPoint = glm::min(p1, glm::min(p2, p3));
            glm::vec3 maxPoint = glm::max(p1, glm::max(p2, p3));

            boundingBoxes.push_back(new BoundingBox(minPoint, maxPoint));
        }
    }

    // render the mesh
    void Draw(Shader &shader, bool depthOnly = false)
    {
        glBindVertexArray(VAO);

        if (!textures.empty())
        {
            const std::string &type = textures[0].type;
            GLuint texID = textures[0].id;
            std::string materialName = "material";

            glActiveTexture(GL_TEXTURE0);
            shader.setInt(materialName + ".diffuse", 0);
            shader.setFloat(materialName + ".shininess", 3.0f);
            glBindTexture(GL_TEXTURE_2D, texID);
        }
        else
        {
            // Set fallback values or unbind texture
            shader.setFloat("material.shininess", 3.0f);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                       GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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
        // A great thing about structs is that their memory layout is sequential
        // for all its items. The effect is that we can simply pass a pointer to
        // the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
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
        /*
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex),
                               (void *)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, m_Weights));
        */
        glBindVertexArray(0);
    }
};
#endif
