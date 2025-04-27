#include "Entity.h"
#include "BoundingTree.h"

Entity::Entity()
{
    modelMatrix = glm::mat4(1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    parent = nullptr;
    model = nullptr;
}

void Entity::init()
{
}

void Entity::update()
{
}
