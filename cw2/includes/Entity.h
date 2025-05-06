#ifndef ENTITY_H
#define ENTITY_H

#include "BoundingTree.h"
#include "Light.h"
#include "Model.h"
#include "Octree.h"

class Entity
{
  public:
    string model;
    int levels = 1;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 position;
    glm::vec3 rotation;
    Octree *ot;

    Light *light;

    vector<Entity *> children;
    Entity *parent;

    Entity();
    Entity(glm::vec3 position, glm::vec3 rotation, string model);
    Entity(Entity *e, string model);
    virtual void init();
    virtual void update(float dt);
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
