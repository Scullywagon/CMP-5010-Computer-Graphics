#ifndef TERRAIN_H
#define TERRAIN_H

#include "Model.h"
#include "glm/detail/type_vec.hpp"

struct Terrain
{
    Model *raisedTerrain =
        new Model("assets/terrain/RaisedForrest/RaisedTerrain.obj", 45);
    Model *trees = new Model("assets/terrain/RaisedForrest/Untitled.obj", 1.5);
    glm::vec4 translations[1000];
    int forrestCount;

    Terrain(Shader &shader)
    {
        int index = 0;
        for (int x = -300; x <= 300 && index < 1000; x += 25)
        {
            for (int z = -300; z <= 300 && index < 1000; z += 25)
            {
                if (glm::length(glm::vec2(x, z)) >= 100.0f)
                {
                    float randVal = (rand() % 7) - 2;
                    float r = (rand() % 100) / 100.0f;
                    translations[index++] = glm::vec4(x, randVal, z, r);
                }
            }
        }
        forrestCount = index;
    }

    void draw(Shader &shader)
    {
        unsigned int offsetLoc = glGetUniformLocation(shader.ID, "offsets");
        glUniform4fv(offsetLoc, 1000, &translations[0].x);
        raisedTerrain->DrawInstanced(shader, forrestCount);
        trees->DrawInstanced(shader, forrestCount);
    }
};

#endif
