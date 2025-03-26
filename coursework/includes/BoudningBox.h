#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>

class BoundingBox
{
  public:
    glm::vec3 min;
    glm::vec3 max;

    BoundingBox(glm::vec3 min, glm::vec3 max) : min(min), max(max)
    {
    }

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
        min *= scale;
        max *= scale;
    }

    void translate(glm::vec3 translation)
    {
        min += translation;
        max += translation;
    }

    void rotate(float degrees, glm::vec3 axis)
    {
        glm::mat4 rotation =
            glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
        min = glm::vec3(rotation * glm::vec4(min, 1.0f));
        max = glm::vec3(rotation * glm::vec4(max, 1.0f));
    }

  private:
};

#endif
