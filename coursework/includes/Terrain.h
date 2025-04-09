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

    Terrain(Shader &shader)
    {
        int index = 0;
        for (int x = -700; x <= 700 && index < 1000; x += 35)
        {
            for (int z = -700; z <= 700 && index < 1000; z += 35)
            {
                if (glm::length(glm::vec2(x, z)) >= 100.0f)
                {
                    float randVal = (rand() % 7) - 2;
                    float r = (rand() % 100) / 100.0f;
                    translations[index++] = glm::vec4(x, randVal, z, r);
                }
            }
        }
    }

    void draw(Shader &shader)
    {
        unsigned int offsetLoc = glGetUniformLocation(shader.ID, "offsets");
        glUniform4fv(offsetLoc, 1000, &translations[0].x);
        raisedTerrain->DrawInstanced(shader, 1000);
        trees->DrawInstanced(shader, 1000);
    }
};

#endif
