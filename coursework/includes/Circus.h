#ifndef CIRCUS_H
#define CIRCUS_H

#include "Light.h"
#include "Model.h"
struct Circus
{
    Model *tent = new Model("assets/circus/tent.obj", 1.0f);
    Model *eye = new Model("assets/circus/eye.obj", 1.0f);
    Model *stage = new Model("assets/stage/stage.obj", 0.03f);

    glm::vec3 position = glm::vec3(35.0f, 0.3f, -35.0f);

    Light *light = new Light(
        position + glm::vec3(-3.0f, 21.5f, -15.0f), glm::vec3(0.1f, 0.10f, 0.20f),
        glm::vec3(0.3f, 0.4f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.10f,
        0.02f);

    Circus()
    {
        tent->translate(position);
        eye->translate(position);
        stage->translate(position + glm::vec3(0.0f, 0.3f, -2.0f));
    }

    void draw(Shader &shader, bool depthOnly = false)
    {
        stage->Draw(shader, depthOnly);
        tent->Draw(shader, depthOnly);
        shader.setBool("isLight", true);
        shader.setVec3("outputColor", glm::vec3(0.3f, 0.4f, 0.6f));
        eye->Draw(shader, depthOnly);
        shader.setBool("isLight", false);
    }
};

#endif
