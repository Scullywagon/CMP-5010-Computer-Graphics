#ifndef TERRAIN_H
#define TERRAIN_H

#include "Model.h"

struct Terrain
{
    Model *raisedTerrain =
        new Model("assets/terrain/RaisedForrest/RaisedTerrain.obj", 60);
    vector<glm::vec3> translations;

    Terrain(Shader &shader)
    {
        for (int x = -500; x <= 500; x += 30)
        {
            for (int z = -500; z <= 500; z += 30)
            {
                if (glm::length(glm::vec2(x, z)) >= 70.0f)
                {
                    translations.emplace_back(x, 0.0f, z);
                    if (translations.size() == 1000)
                        break;
                }
            }
        }
    }

    void draw(Shader &shader)
    {
        for (int i = 0; i < translations.size(); i++)
        {
            shader.setVec3(("offsets[" + std::to_string(i) + "]"),
                           translations[i]);
        }
        raisedTerrain->DrawInstanced(shader, translations.size());
    }
};

#endif
