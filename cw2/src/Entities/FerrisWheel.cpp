#include "Entities/FerrisWheel.h"
#include "Entity.h"
#include "glm/detail/type_vec.hpp"

using glm::radians;

Stand::Stand()
{
    model = "Stand";
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 5.0f, 0.0f);
}

void Stand::init()
{
    modelMatrix = glm::translate(modelMatrix, this->position);
    children.push_back(new Wheel(this->position));
    childUpdate();
}

void Stand::update()
{
}

Wheel::Wheel(glm::vec3 parent)
{
    model = "Wheel";
    this->position = glm::vec3(-2.7f, 24.96f, 0.0f);
    this->position += parent;
}

void Wheel::init()
{
    modelMatrix = glm::translate(modelMatrix, this->position);
    createCarts();
    childUpdate();
}

void Wheel::update()
{
}

void Wheel::createCarts()
{
    glm::vec3 initPos = glm::vec3(0.0f, -19.6484f, 0.09f);
    glm::mat4 rotate = glm::mat4(1.0f);
    /*
    children.push_back(
        new Cart(this->position, initPos, &this->rotationMatrix));
    */

    for (int i = 0; i < 12; i++)
    {
        rotate = glm::rotate(rotate, radians(-30.000002f),
                             glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec3 newPos = glm::vec3(rotate * glm::vec4(initPos, 0.0f));
        children.push_back(
            new Cart(this->position, newPos, &this->rotationMatrix));
        cout << newPos.x << " " << newPos.y << " " << newPos.z << endl;
    }
}

Cart::Cart(glm::vec3 parent, glm::vec3 localPos, glm::mat4 *rotationMat)
{
    model = "Cart";
    this->position = localPos;
    this->position += parent;
    this->rotationMatrix = rotationMat;
}

void Cart::init()
{
    modelMatrix = glm::translate(modelMatrix, this->position);
}

void Cart::update()
{
}
