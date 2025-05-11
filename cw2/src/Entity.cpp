#include "Entity.h"
#include "BoundingTree.h"
#include "Octree.h"

Entity::Entity()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    localRotation = glm::mat4(1.0f);
    parent = nullptr;
    light = nullptr;
    model = "";
}

Entity::Entity(glm::vec3 position, glm::mat4 rotation, string model)
{
    this->position = position;
    this->localRotation = rotation;
    this->model = model;
    parent = nullptr;
    light = nullptr;
}

Entity::Entity(Entity *e, string model)
{
    this->position = e->position;
    this->localRotation = e->localRotation;
    this->model = model;
    parent = nullptr;
    light = nullptr;
}

void Entity::init()
{
    modelMatrix = glm::translate(modelMatrix, position);

    /*
    for (int i = 0; i < 3; i++)
    {
        modelMatrix = glm::rotate(modelMatrix, rotation[i], glm::vec3(1.0f));
    }
    */
}

void Entity::update(float dt)
{
}

void Entity::genBoundingTree(Model &model)
{
    this->ot = new Octree(&model, this->modelMatrix, this->levels);
}
