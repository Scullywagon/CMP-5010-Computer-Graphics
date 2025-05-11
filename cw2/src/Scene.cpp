#include "Scene.h"
#include "BoundingTree.h"
#include "CollisionManager.h"
#include "Entities/FerrisWheel.h"
#include "Entity.h"
#include "Light.h"
#include "Skybox.h"

Scene::Scene()
    : worldLight(glm::vec3(0.2, -0.8, 0.2), glm::vec3(0.1, 0.1, 0.1),
                 glm::vec3(0.1, 0.1, 0.2), glm::vec3(0.4, 0.4, 0.4))
{
    assets["Stand"] = new Model("assets/base/base.obj", 2.0f);
    assets["Crate"] =
        new Model("assets/animalCrate/circus_animal_crate.obj", 1.0f);
    assets["Wheel"] = new Model("assets/wheel2/wheel2.obj", 2.0f);
    assets["Cart"] = new Model("assets/Cart/1.obj", 2.0f);
    assets["Terrain"] =
        new Model("assets/terrain/RaisedForrest/RaisedTerrain.obj", 45.0f);
    assets["Tree"] =
        new Model("assets/terrain/RaisedForrest/Untitled2.obj", 1.5f);
    assets["OilLamp"] = new Model("assets/oilLamp/oilLamp.obj", 8.0f);
    assets["OilLampGlass"] = new Model("assets/oilLamp/oilLampGlass.obj", 8.0f);
    assets["OilLampGlass"]->isLight = true;
    assets["OilLampGlass"]->outColor = glm::vec3(0.8f, 0.8f, 0.2f);
    assets["CircusTent"] = new Model("assets/circus/tent.obj", 1.0f);
    assets["CircusEye"] = new Model("assets/circus/eye.obj", 1.0f);
    assets["CircusEye"]->isLight = true;
    assets["CircusEye"]->outColor = glm::vec3(0.5f, 0.8f, 0.9f);
    collisionManager = new CollisionManager(&Player);
    generateTerrain();
}

void Scene::init()
{
    this->stand = new Stand();
    entities.push_back(this->stand);
    Entity *e = new Entity(glm::vec3(-20.0f, 0.8f, -30.0f), glm::mat4(0.0f),
                           "CircusTent");
    e->children.push_back(new Entity(e, "CircusEye"));
    e->children[0]->light =
        new Light(glm::vec3(-23.0f, 24.0f, -7.0f), glm::vec3(0.0f, 0.0f, 0.5f),
                  glm::vec3(0.6f, 0.7f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                  1.0f, 0.1f, 0.01f);
    entities.push_back(e);

    for (auto &entity : entities)
    {
        addEntities(*entity);
    }
    cout << collisionManager->xList.size() << endl;
    collisionManager->init();
}

void Scene::update(float dt)
{
    if (!Player.inFerrisWheel)
        collisionManager->update();

    for (auto &entity : entities)
    {
        entity->update(dt);
    }
    cam->move();
}

void Scene::playerActivate()
{
    if (stand->ot->colliding)
    {
        if (Player.inFerrisWheel)
        {
            Player.exitFerrisWheel();
        }
        else
        {
            Player.enterFerrisWheel(stand->children[0]->children[0]);
        }
    }
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

void Scene::addEntities(Entity &entity)
{
    entity.init();
    Model *model = assets[entity.model];
    translations[model].push_back(&entity.modelMatrix);
    if (entity.light != nullptr)
    {
        lights.push_back(entity.light);
    }
    if (entity.model != "Tree" && entity.model != "OilLamp" &&
        entity.model != "OilLampGlass")
    {
        entity.genBoundingTree(*model);
        collisionManager->add(entity.ot);
    }
    for (Entity *child : entity.children)
    {
        addEntities(*child);
    }
}

void Scene::generateTerrain()
{
    int scale = 80;
    int index = 0;
    for (int x = -scale; x <= scale && index < 10000; x += 25)
    {
        for (int z = -scale; z <= scale && index < 10000; z += 25)

        {
            if (glm::length(glm::vec2(x, z)) >= 70.0f)
            {
                float y = (rand() % 7) - 2;
                float r = (rand() % 100) / 100.0f;
                glm::vec3 translation = glm::vec3(x, y, z);
                entities.push_back(
                    new Entity(translation, glm::mat4(0.0f), "Terrain"));
                entities.push_back(
                    new Entity(translation, glm::mat4(0.0f), "Tree"));
                index++;
            }
        }
    }
}
