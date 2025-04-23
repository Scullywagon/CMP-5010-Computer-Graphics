#ifndef ENTITY_H
#define ENTITY_H

#include "Model.h"

class Entity
{
  public:
    Model *model; 
    glm::mat4 modelMatrix;
    glm::vec3 position;
    glm::vec3 rotation;
    // BoundingTree bt;

    // Light *light;

    vector<Entity *> children;
    Entity *parent;

    Entity();
    virtual void init();
    virtual void update();
    ~Entity() = default;

  private:
};

#endif
