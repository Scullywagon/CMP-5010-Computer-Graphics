#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include "Entity.h"

class Stand : public Entity
{
  public:
    Stand();
    void init() override;
    void update() override;

  private:
};

class Wheel : public Entity
{
  public:
    bool enableRotation = false;
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    Wheel(glm::vec3 parent);
    void init() override;
    void update() override;

  private:
    void createCarts();
};

class Cart : public Entity
{
  public:
    glm::mat4 *rotationMatrix;
    Cart(glm::vec3 parent, glm::vec3 localPos, glm::mat4 *rotationMat);
    void init() override;
    void update() override;

  private:
};

#endif
