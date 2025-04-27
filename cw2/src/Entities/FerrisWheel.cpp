#include "Entities/FerrisWheel.h"

Stand::Stand(Model *mod)
{
    this->model = mod;
    this->position = glm::vec3(2.7f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 5.0f, 0.0f);
}

void Stand::init()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, this->position);
    bt = new BoundingTree(*model, &modelMatrix);
}

void Stand::update()
{
}

Wheel::Wheel(Model *mod)
{
    this->model = mod;
    this->position = glm::vec3(0.0f, 24.96f, 0.0f);
}

void Wheel::init()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, this->position);
    bt = new BoundingTree(*model, &modelMatrix);
}

void Wheel::update()
{
}
