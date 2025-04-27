#include "Entities/FerrisWheel.h"

Stand::Stand(Model *mod)
{
    this->model = mod;
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(20.0f, 0.0f, 0.0f);
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
