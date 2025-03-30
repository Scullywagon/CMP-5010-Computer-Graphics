#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include "Model.h"
#include "ParentCamera.h"
#include "glm/detail/type_vec.hpp"
#include <vector>

class FerrisWheelCam : public ParentCamera
{
  public:
    glm::vec3 Position;
    const float SPEED = 0.0f;
    const float FOV = 75.0f;
    FerrisWheelCam(glm::vec3 cartPosition)
    {
        // Initialize the base class members directly
        Position = cartPosition + glm::vec3(1.0f, -0.5f, 0.5f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Right = glm::vec3(1.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        Yaw = YAW;
        Pitch = PITCH;

        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;

        fov = glm::radians(FOV);
    }

    glm::mat4 GetViewMatrix() override
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void processInput(movement_dir dir, float deltaTime) override
    {
    }

    void ProcessMouseMovement(float xoffset, float yoffset) override
    {
        // change in mouse position combined with sensitivity
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        // update camera yaw and pitch
        Yaw += xoffset;
        Pitch += yoffset;

        // ensure the pitch is constrained to avoid flipping
        if (CONSTRAIN_PITCH)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -77.0f)
                Pitch = -77.0f;
        }

        updateCameraVectors();
    }

  private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

struct Cart
{
    Model cart;
    glm::vec3 position;
    glm::vec3 offSet;

    FerrisWheelCam *camera;

    Cart(glm::vec3 position, glm::vec3 offset)
        : cart(Model("assets/Cart/1.obj"))
    {
        cart.scale(2.0f);
        this->position = position;
        this->offSet = offset;
        cart.translate(position);
        camera = new FerrisWheelCam(position);
        cart.scale(0.9);
    }

    void updateCamera(glm::vec3 translation)
    {
        camera->Position += translation * 2.0f;
    }

    void rotate(glm::mat4 rotationMatrix, glm::vec3 center)
    {
        cart.scale(1.11111111111f); // puts it back at 2.0
        glm::vec3 newPosition =
            glm::vec3(rotationMatrix * glm::vec4(offSet, 1.0f)) + center;
        glm::vec3 translation = newPosition - position;
        position = newPosition;
        cart.translate(translation);
        updateCamera(translation);
        cart.scale(0.9);
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

    CollisionManager *collisionManager;

    glm::vec3 center;
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    float speed = 20.1f; // effectively this will be angle * deltaTime (if
                         // time is 1 then this will be the angle)

    FerrisWheel(CollisionManager *collisionManager)
        : stand(Model("assets/base/base.obj")),
          wheel(Model("assets/wheel2/wheel2.obj"))
    {
        this->collisionManager = collisionManager;
        stand.scale(2.0f);
        wheel.scale(2.0f);
        stand.translate(glm::vec3(1.35f, 0.0f, 0.0f));
        wheel.translate(glm::vec3(0.0f, 12.48f, 0.0f));
        carts = {
            Cart(glm::vec3(0.0f, 2.5958f, 0.045f), // 2.6558f
                 glm::vec3(0.0f, -9.8242f, 0.045f)),
            Cart(glm::vec3(0.0f, 3.9378f, -4.8579f),
                 glm::vec3(0.0f, -8.5422f, -4.8579f)),
            Cart(glm::vec3(0.0f, 7.4953, -8.4636f), // 6
                 glm::vec3(0.0f, -4.9847f, -8.4636f)),
            Cart(glm::vec3(0.0f, 12.399f, -9.8097f),
                 glm::vec3(0.0f, -0.081f, -9.8097f)),
            Cart(glm::vec3(0.0f, 17.279, -8.5838),
                 glm::vec3(0.0f, 4.799f, -8.5838)),
            Cart(glm::vec3(0.0f, 20.932, -5.0262),
                 glm::vec3(0.0f, 8.452f, -5.0262)),
            Cart(glm::vec3(0.0f, 22.303f, -0.0504f),
                 glm::vec3(0.0f, 9.823f, -0.0504)),
            Cart(glm::vec3(0.0f, 21.077f, 4.7812f),
                 glm::vec3(0.0f, 8.597f, 4.7812f)),
            Cart(glm::vec3(0.0f, 17.519f, 8.459f),
                 glm::vec3(0.0f, 5.039f, 8.459f)),
            Cart(glm::vec3(0.0f, 12.639f, 9.8051f),
                 glm::vec3(0.0f, 0.159f, 9.8051f)),
            Cart(glm::vec3(0.0f, 7.6646, 8.5311),
                 glm::vec3(0.0f, -4.8154f, 8.5311)),
            Cart(glm::vec3(0.0f, 4.082, 4.9735),
                 glm::vec3(0.0f, -8.398f, 4.9735)),
        };
        center = glm::vec3(0.0f, 12.48f, 0.0f);

        // collisionManager->boundingBoxes.push_back(stand.boundingBox);
        //  collisionManager->boundingBoxes.push_back(wheel.boundingBox);
        for (auto &cart : carts)
        {
            collisionManager->boundingBoxes.push_back(cart.cart.boundingBox);
        }
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
