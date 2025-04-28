#ifndef ENTITY_H
#define ENTITY_H

#include "BoundingTree.h"
#include "Model.h"

class Entity
{
  public:
    string model;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 position;
    glm::vec3 rotation;
    BoundingTree *bt;

    // Light *light;

    vector<Entity *> children;
    Entity *parent;

    Entity();
    virtual void init();
    virtual void update();
    void genBoundingTree(Model &model);
    ~Entity() = default;

  protected:
    void childUpdate()
    {
        for (Entity *child : children)
        {
            child->parent = this;
        }
    }
};

#endif
