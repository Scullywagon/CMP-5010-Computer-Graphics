#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include "Constants.h"
#include "Entity.h"
#include "VarName.h"

class Stand : public Entity
{
  public:
    Stand();
    void init() override;
    void update(float dt) override;

  private:
};

class Wheel : public Entity
{
  public:
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    Wheel(glm::vec3 parent);
    void init() override;
    void update(float dt) override;
    // void startRotation();

  private:
    void createCarts();
    void rotate(float dt);
    float currentSpeed = 0.0f;
    const float speed = 2.1f;
};

class Cart : public Entity
{
  public:
    glm::vec3 center;
    glm::vec3 localPos;
    glm::mat4 *rotationMatrix;
    Cart(glm::vec3 parent, glm::vec3 localPos, glm::mat4 *rotationMat);
    void init() override;
    void update(float dt) override;

  private:
};

#endif
