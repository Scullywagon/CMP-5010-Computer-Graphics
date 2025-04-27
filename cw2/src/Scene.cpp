#include "Scene.h"
#include "CollisionManager.h"
#include "Entities/FerrisWheel.h"
#include "Light.h"
#include "Skybox.h"

Scene::Scene()
    : worldLight(glm::vec3(0.2, -0.8, 0.2), glm::vec3(0.4, 0.4, 0.4),
                 glm::vec3(0.6, 0.6, 0.6), glm::vec3(1.0, 1.0, 1.0))
{
    assets["Stand"] = new Model("assets/base/base.obj", 2.0f);
    assets["Crate"] =
        new Model("assets/animalCrate/circus_animal_crate.obj", 1.0f);
    assets["Wheel"] = new Model("assets/wheel2/wheel2.obj", 2.0f);
    collisionManager = new CollisionManager(&cam);
}

void Scene::init()
{
    Stand *stand = new Stand(assets["Stand"]);
    Wheel *wheel = new Wheel(assets["Wheel"]);
    entities.push_back(stand);
    entities.push_back(wheel);

    for (auto &entity : entities)
    {
        entity->init();
        Model *model = entity->model;
        translations[model].push_back(&entity->modelMatrix);
        collisionManager->add(entity->bt);
    }
}

void Scene::update()
{
    collisionManager->update();
    cam.move();
}

void Scene::testModels(string name)
{
    translations[assets[name]].push_back(new glm::mat4(1.0f));
    int scale = 90;
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
                instanceMat =
                    glm::rotate(instanceMat, r, glm::vec3(0.0f, 1.0f, 0.0f));
                translations[assets[name]].push_back(
                    new glm::mat4(instanceMat));
                index++;
            }
        }
    }
}
