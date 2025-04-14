#ifndef OILLAMP_H
#define OILLAMP_H

#include "Light.h"
#include "Model.h"

struct OilLamp
{
    Model *lamp = new Model("assets/oilLamp/oilLamp.obj", 7.0f, false);
    Model *glass = new Model("assets/oilLamp/oilLampGlass.obj", 7.0f, false);
    Light *light;
    
    glm::vec3 position;

    glm::vec3 ambient = glm::vec3(0.1f, 0.05f, 0.01f);
    glm::vec3 diffuse = glm::vec3(1.0f, 0.7f, 0.2f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float constant = 1.0f;
    float linear = 0.10f;
    float quadratic = 0.02f;
    float flicker;

    OilLamp(glm::vec3 position)
    {
        this->position = position;

        lamp->translate(position);
        glass->translate(position);
        light = new Light(position, ambient, diffuse, specular,
                           constant, linear, quadratic);
    }

    void translate(glm::vec3 translation)
    {
        lamp->translate(translation);
        glass->translate(translation);
        light->position += translation;
    }

    void draw(Shader &shader)
    {
        flicker = 0.4f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.6f));
        lamp->Draw(shader);
        shader.setBool("isLight", true);
        shader.setVec3("outputColor", (diffuse * flicker));
        light->diffuse = diffuse * flicker;
        glass->Draw(shader);
        shader.setBool("isLight", false);
    }

};

#endif
