#include "Entities/FerrisWheel.h"
#include "Entity.h"
#include "glm/detail/type_vec.hpp"

using glm::radians;

Stand::Stand()
{
    model = "Stand";
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 5.0f, 0.0f);
    this->levels = 1;
}

void Stand::init()
{
    modelMatrix = glm::translate(modelMatrix, this->position);
    children.push_back(new Wheel(this->position));
}

void Stand::update(float dt)
{
    for (Entity *child : children)
    {
        child->update(dt);
    }
}

Wheel::Wheel(glm::vec3 parent)
{
    model = "Wheel";
    this->levels = 1;
    this->position = glm::vec3(-2.7f, 24.96f, 0.0f);
    this->position += parent;
}

void Wheel::init()
{
    modelMatrix = glm::translate(modelMatrix, this->position);
    createCarts();
}

void Wheel::update(float dt)
{
    if (vars::enableRotation)
    {
        rotate(dt);
    }
    for (Entity *child : children)
    {
        child->update(dt);
    }
}

void Wheel::rotate(float dt)
{
    float rotation = dt * speed;
    this->modelMatrix = glm::rotate(modelMatrix, radians(rotation),
                                    glm::vec3(1.0f, 0.0f, 0.0f));
    this->rotationMatrix = glm::rotate(rotationMatrix, radians(rotation),
                                       glm::vec3(1.0f, 0.0f, 0.0f));
    ot->rotate(rotation, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Wheel::createCarts()
{
    glm::vec3 vec1(0.0f, -19.6484f, 0.090f);
    glm::vec3 vec2(0.0f, -17.0844f, -9.7158f);
    glm::vec3 vec3(0.0f, -9.9694f, -16.9272f);
    glm::vec3 vec4(0.0f, -0.162f, -19.6194f);
    glm::vec3 vec5(0.0f, 9.598f, -17.1676f);
    glm::vec3 vec6(0.0f, 16.904f, -10.0524f);
    glm::vec3 vec7(0.0f, 19.646f, -0.1008f);
    glm::vec3 vec8(0.0f, 17.194f, 9.5624f);
    glm::vec3 vec9(0.0f, 10.078f, 16.918f);
    glm::vec3 vec10(0.0f, 0.318f, 19.6102f);
    glm::vec3 vec11(0.0f, -9.6308f, 17.0622f);
    glm::vec3 vec12(0.0f, -16.796f, 9.947f);

    children.push_back(new Cart(this->position, vec1, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec2, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec3, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec4, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec5, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec6, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec7, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec8, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec9, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec10, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec11, &this->rotationMatrix));
    children.push_back(new Cart(this->position, vec12, &this->rotationMatrix));
}

Cart::Cart(glm::vec3 parent, glm::vec3 localPos, glm::mat4 *rotationMat)
{
    model = "Cart";
    this->levels = 4;
    this->position = parent + localPos;
    this->center = parent;
    this->localPos = localPos;
    this->rotationMatrix = rotationMat;
}

void Cart::init()
{
    modelMatrix = glm::translate(modelMatrix, this->position);
    children.push_back(new Entity(this->position - glm::vec3(1.0f, 4.5f, 0.0f),
                                  glm::vec3(0.0f), "OilLamp"));
    children.push_back(new Entity(this->position - glm::vec3(1.0f, 4.5f, 0.0f),
                                  glm::vec3(0.0f), "OilLampGlass"));
    children[1]->light = new Light(
        children[1]->position, glm::vec3(0.1f, 0.05f, 0.01f),
        glm::vec3(1.0f, 0.7f, 0.2f), glm::vec3(1.0f), 1.0f, 0.1f, 0.02f);
}

void Cart::update(float dt)
{
    if (vars::enableRotation)
    {
        glm::vec3 newPos =
            glm::vec3((*rotationMatrix) * glm::vec4(localPos, 0.0f)) + center;
        glm::vec3 translation = newPos - position;
        position = newPos;
        this->modelMatrix = glm::translate(modelMatrix, translation);
        ot->translate(translation);
        for (Entity *child : children)
        {
            child->modelMatrix =
                glm::translate(child->modelMatrix, translation);
        }
    }
}
