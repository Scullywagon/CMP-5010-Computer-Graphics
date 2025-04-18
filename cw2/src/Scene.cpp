#include "Scene.h"

Scene::Scene()
{
    Model *mod = new Model("assets/animalCrate/circus_animal_crate.obj", 1.0f);
    cout << "creating scene" << endl;
    assets["cart"] = mod;
    cout << "cart created" << endl;
    glm::mat4 model = glm::mat4(1.0f);
    translations[mod].push_back(model);
    cout << "Scene initialized" << endl;
}

void Scene::init()
{

}

void Scene::update()
{
}
