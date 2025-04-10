#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GL/glew.h>
#include <iostream>
#include <vector>

struct BoundingBox
{
  public:
    glm::vec3 min;
    glm::vec3 max;

    float scaleVal = 1.0f;

    bool sorted = false;

    bool checkSubBoxes = true;
    std::vector<BoundingBox *> subBoxes;

    // generates the box from those values
    BoundingBox(glm::vec3 min, glm::vec3 max) : min(min), max(max)
    {
        setupGL();
    }

    // if its colliding then return false
    // for the camera this will likely result in a normal calulation
    bool isColliding(BoundingBox &other)
    {
        if (max.x < other.min.x || min.x > other.max.x)
        {
            return false;
        }
        if (max.y < other.min.y || min.y > other.max.y)
        {
            return false;
        }
        if (max.z < other.min.z || min.z > other.max.z)
        {
            return false;
        }
        return true;
    }

    void scale(float scale)
    {
        glm::mat4 scaleMatrix =
            glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        min = glm::vec3(scaleMatrix * glm::vec4(min, 1.0f));
        max = glm::vec3(scaleMatrix * glm::vec4(max, 1.0f));
        scaleVal *= scale;
        sorted = false;
    }

    void translate(glm::vec3 translation)
    {
        glm::vec3 scaledTranslation = translation * scaleVal;
        min += scaledTranslation;
        max += scaledTranslation;
        sorted = false;

        for (BoundingBox *box : subBoxes)
        {
            box->translate(translation);
        }
    }

    void rotate(float degrees, glm::vec3 axis)
    {
        /*
        glm::mat4 rotation =
            glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
        min = glm::vec3(rotation * glm::vec4(min, 1.0f));
        max = glm::vec3(rotation * glm::vec4(max, 1.0f));
        sorted = false;
        */
    }
    unsigned int VAO, VBO, EBO;
    void setupGL()
    {
        // Vertices: Two points, one at 'min' and one at 'max'
        std::vector<glm::vec3> vertices = {
            min, // First point (min)
            max  // Second point (max)
        };

        // Indices: Only one line, connecting the two points
        std::vector<unsigned int> indices = {0, 1}; // Line from 0 to 1

        // Create VAO, VBO, and EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // VBO for the vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                     vertices.data(), GL_STATIC_DRAW);

        // EBO for the indices (line)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     indices.data(), GL_STATIC_DRAW);

        // Vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    // Draw the line from 'min' to 'max'
    void draw(GLuint shaderProgram)
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT,
                       0); // 2 is the number of indices to draw the line
        glBindVertexArray(0);
    }

  private:
};

#endif
