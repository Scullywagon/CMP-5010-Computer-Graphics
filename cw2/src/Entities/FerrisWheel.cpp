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
    modelMatrix =
        glm::rotate(modelMatrix, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(100.5f, 0.5f, 0.5f));
}

void Stand::update()
{
}
