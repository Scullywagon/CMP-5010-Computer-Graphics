#ifndef TERRAIN_H
#define TERRAIN_H

#include "Model.h"
#include "glm/detail/type_vec.hpp"

struct Terrain
{
    Model *raisedTerrain =
        new Model("assets/terrain/RaisedForrest/RaisedTerrain.obj", 60);
    Model *flatTerrain =
        new Model("assets/terrain/FloorTile/FloorTile.obj", 60);
    glm::vec3 translations[1000];
    glm::vec3 flatT[1000];
    int raisedCount;
    int flatCount;

    Terrain(Shader &shader)
    {
        int index = 0;
        for (int x = -700; x <= 700 && index < 1000; x += 35)
        {
            for (int z = -700; z <= 700 && index < 1000; z += 35)
            {
                if (glm::length(glm::vec2(x, z)) >= 70.0f)
                {
                    translations[index++] = glm::vec3(x, 0.0f, z);
                }
            }
        } 

        index = 0;
        for (int x = -70; x <= 70; x += 20)
        {
            for (int z = -70; z <= 70; z += 20)
            {
                flatT[index++] = glm::vec3(x, 0.0f, z);
            }
        }
    }

    void draw(Shader &shader)
    {
        unsigned int offsetLoc = glGetUniformLocation(shader.ID, "offsets");
        glUniform3fv(offsetLoc, 1000, &translations[0].x);
        raisedTerrain->DrawInstanced(shader, 10);
        flatTerrain->Draw(shader);
    }
};

#endif
