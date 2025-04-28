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
}

void Wheel::update()
{
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
    /*
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
    */
}

Cart::Cart(glm::vec3 parent, glm::vec3 localPos, glm::mat4 *rotationMat)
{
    model = "Cart";
    position = localPos;
    this->rotationMatrix = rotationMat;
}

void Cart::init()
{
    modelMatrix = glm::translate(modelMatrix, this->position);
}

void Cart::update()
{
}
