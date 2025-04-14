#ifndef CIRCUS_H
#define CIRCUS_H

#include "Light.h"
#include "Model.h"
#include "OilLamp.h"

struct Circus
{
    Model *tent = new Model("assets/circus/tent.obj", 1.0f, false);
    Model *eye = new Model("assets/circus/eye.obj", 1.0f, false);
    Model *stage = new Model("assets/stage/stage.obj", 0.03f, false);
    Model *props = new Model("assets/props/props.obj", 2.0f, false);
    Model *sign = new Model("assets/sign1/sign1.obj", 1.5f, false);
    Model *trailer =
        new Model("assets/circusWagons/circus_wagons.obj", 80.0f, false);

    vector<Model *> animalCrates;
    vector<OilLamp *> lamps;

    glm::vec3 position = glm::vec3(25.0f, 0.3f, -35.0f);

    Light *light =
        new Light(position + glm::vec3(-3.0f, 21.5f, 16.0f),
                  glm::vec3(0.1f, 0.10f, 0.20f), glm::vec3(0.3f, 0.4f, 0.7f),
                  glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.20f, 0.05f);

    Circus()
    {
        tent->translate(position);
        tent->boundingBox->checkSubBoxes = false;
        eye->translate(position);
        eye->boundingBox->checkSubBoxes = false;
        stage->translate(position + glm::vec3(0.0f, 0.3f, -2.0f));
        tent->boundingBox->checkSubBoxes = false;
        props->translate(position + glm::vec3(5.0f, -0.3f, 22.0f));
        sign->translate(position + glm::vec3(-5.0f, -0.3f, 22.0f));
        trailer->translate(position + glm::vec3(-35.0f, -0.3f, -25.0f));

        animalCrates = {new Model("assets/animalCrate/circus_animal_crate.obj",
                                  0.01f, false),
                        new Model("assets/animalCrate/circus_animal_crate.obj",
                                  0.01f, false),
                        new Model("assets/animalCrate/circus_animal_crate.obj",
                                  0.01f, false),
                        new Model("assets/animalCrate/circus_animal_crate.obj",
                                  0.01f, false)};
        animalCrates[0]->translate(position + glm::vec3(-30.0f, 0.9f, -18.0f));
        animalCrates[1]->translate(position + glm::vec3(-35.0f, 0.9f, -18.0f));
        animalCrates[2]->translate(position + glm::vec3(-32.0f, 2.8f, -18.0f));
        animalCrates[3]->translate(position + glm::vec3(-35.0f, 0.0f, -9.0f));

        float rad = 14.0f;

        for (int i = 0; i < 12; i++)
        {
            float angle = glm::radians(i * 360.0f / 12.0f);
            float x = rad * cos(angle);
            float z = rad * sin(angle);
            float y = 0.0f;
            lamps.push_back(new OilLamp(position + glm::vec3(x, y, z)));
        }
    }

    void draw(Shader &shader, bool depthOnly = false)
    {
        stage->Draw(shader, depthOnly);
        tent->Draw(shader, depthOnly);
        shader.setBool("isLight", true);
        shader.setVec3("outputColor", glm::vec3(0.3f, 0.4f, 0.6f));
        eye->Draw(shader, depthOnly);
        shader.setBool("isLight", false);
        for (auto &lamp : lamps)
        {
            lamp->draw(shader);
        }
        for (auto &crate : animalCrates)
        {
            crate->Draw(shader, depthOnly);
        }
        props->Draw(shader, depthOnly);
        sign->Draw(shader);
        trailer->Draw(shader, depthOnly);
    }
};

#endif
