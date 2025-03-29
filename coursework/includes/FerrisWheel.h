#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include "Model.h"
#include "glm/detail/type_vec.hpp"
#include <vector>

struct Cart
{
    Model cart;
    glm::vec3 position;
    glm::vec3 offSet;

    Cart(glm::vec3 position, glm::vec3 offset)
        : cart(Model("assets/Cart/1.obj"))
    {
        cart.scale(2.0f);
        this->position = position;
        this->offSet = offset;
        cart.translate(position);
    }

    void rotate(glm::mat4 rotationMatrix, glm::vec3 center)
    {
        glm::vec3 newPosition =
            glm::vec3(rotationMatrix * glm::vec4(offSet, 1.0f)) + center;
        glm::vec3 translation = newPosition - position;
        position = newPosition;
        cart.translate(translation);
    }

    void draw(Shader &shader)
    {
        cart.Draw(shader);
    }
};

struct FerrisWheel
{
    Model stand;
    Model wheel;
    vector<Cart> carts;

    glm::vec3 center;
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    float speed = 10.1f; // effectively this will be angle * deltaTime (if time
                         // is 1 then this will be the angle)

    FerrisWheel()
        : stand(Model("assets/base/base.obj")),
          wheel(Model("assets/wheel2/wheel2.obj"))
    {
        stand.scale(2.0f);
        wheel.scale(2.0f);
        stand.translate(glm::vec3(1.35f, 0.0f, 0.0f));
        wheel.translate(glm::vec3(0.0f, 12.48f, 0.0f));
        carts = {
            Cart(glm::vec3(0.0f, 2.8818f, 0.045f),
                 glm::vec3(0.0f, -9.79f, -0.045f)),
            Cart(glm::vec3(0.0f, 22.47f, -0.05f),
                 glm::vec3(0.0f, 9.79f, -0.05f)),

        };

        center = glm::vec3(0.0f, 12.48f, -0.05f);
    }

    void rotate(float deltaTime)
    {
        float angle = speed * deltaTime;
        glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);

        wheel.rotate(angle, axis);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle), axis);

        for (auto &cart : carts)
        {
            cart.rotate(rotationMatrix, center);
        }
    }

    void draw(Shader &shader, glm::mat4 model)
    {
        stand.Draw(shader);
        shader.setMat4("model", model);
        wheel.Draw(shader);
        shader.setMat4("model", model);
        for (auto &cart : carts)
        {
            cart.draw(shader);
            shader.setMat4("model", model);
        }
    }
};

#endif
