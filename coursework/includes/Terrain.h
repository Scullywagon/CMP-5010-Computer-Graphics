#ifndef TERRAIN_H
#define TERRAIN_H

#include "BoundingBox.h"
#include "Model.h"
#include "glm/detail/func_trigonometric.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Terrain
{
    Model *raisedTerrain =
        new Model("assets/terrain/RaisedForrest/RaisedTerrain.obj", 45, true);
    Model *trees = new Model("assets/terrain/RaisedForrest/Untitled.obj", 1.5, true);
    int forrestCount;
    int scale = 100;

    unsigned int instanceVBO;
    vector<glm::mat4> instanceMatrix;

    Terrain(Shader &shader)
    {
        int index = 0;
        for (int x = -scale; x <= scale && index < 10000; x += 25)
        {
            for (int z = -scale; z <= scale && index < 10000; z += 25)

            {
                if (glm::length(glm::vec2(x, z)) >= 98.0f)
                {
                    float y = (rand() % 7) - 2;
                    float r = (rand() % 100) / 100.0f;
                    glm::mat4 instanceMat = glm::mat4(1.0);
                    instanceMat = glm::translate(instanceMat, glm::vec3(x, y, z));
                    instanceMat = glm::rotate(instanceMat, r, glm::vec3(0.0f, 1.0f, 0.0f));
                    instanceMatrix.push_back(instanceMat);
                    index++;
                }
            }
        }
        forrestCount = index;

        int matsize = sizeof(glm::mat4);
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, forrestCount * matsize, &instanceMatrix[0], GL_STATIC_DRAW);
    }

    void draw(Shader &shader)
    {
        raisedTerrain->DrawInstanced(shader, forrestCount, instanceVBO);
        //trees->DrawInstanced(shader, forrestCount, instanceVBO);
    }
};

#endif
