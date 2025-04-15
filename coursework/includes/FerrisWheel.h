#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include "Light.h"
#include "Model.h"
#include "OilLamp.h"
#include "ParentCamera.h"
#include "glm/detail/type_vec.hpp"
#include <thread>
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
        Position = cartPosition + glm::vec3(1.0f, -2.5f, 1.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Right = glm::vec3(1.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        Yaw = YAW;
        Pitch = PITCH;

        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;

        fov = glm::radians(FOV);

        boundingBox =
            new BoundingBox(Position - glm::vec3(999.5, 999.5, 999.5),
                            Position + glm::vec3(999.5, 999.5, 999.5));
    }

    glm::mat4 GetViewMatrix() override
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void processInput(movement_dir dir, float deltaTime) override
    {
    }
    void move() override
    {
    }

    void ProcessMouseMovement(float xoffset, float yoffset) override
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

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

    OilLamp *lamp;

    glm::vec3 position;
    glm::vec3 offSet;

    FerrisWheelCam *camera;

    Cart(glm::vec3 position, glm::vec3 offset)
        : cart(Model("assets/Cart/1.obj", 1.8f, true))
    {
        this->position = position;
        this->offSet = offset;
        cart.translate(position);
        camera = new FerrisWheelCam(position);
        lamp = new OilLamp(position + glm::vec3(-0.8f, -4.5f, -1.0f));
    }

    void updateCamera(glm::vec3 translation)
    {
        camera->Position += translation;
    }

    void rotate(glm::mat4 rotationMatrix, glm::vec3 center)
    {
        glm::vec3 newPosition =
            glm::vec3(rotationMatrix * glm::vec4(offSet, 1.0f)) + center;
        glm::vec3 translation = newPosition - position;
        position = newPosition;
        cart.translate(translation);
        lamp->translate(translation);
        updateCamera(translation);
    }

    void draw(Shader &shader, bool depthOnly = false)
    {
        cart.Draw(shader, depthOnly);
        lamp->draw(shader);
    }
};

struct TicketBooth
{
    Model *booth =
        new Model("assets/TicketBooth/1970s_ticket_booth.obj", 40.0f, false);
    glm::vec3 position;

    TicketBooth(glm::vec3 position)
    {
        this->position = position;
        booth->translate(position);
    }

    void draw(Shader &shader)
    {
        booth->Draw(shader);
    }
};

struct FerrisWheel
{
    Model stand;
    Model wheel;
    vector<Cart> carts;

    TicketBooth *booth;

    glm::vec3 center;
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    float speed = 20.1f; // effectively this will be angle * deltaTime (if
                         // time is 1 then this will be the angle)

    FerrisWheel()
        : stand(Model("assets/base/base.obj", 2.0f, true)),
          wheel(Model("assets/wheel2/wheel2.obj", 2.0f, true))
    {
        stand.translate(glm::vec3(1.35f * 2.0f, 0.0f, 0.0f));
        wheel.translate(glm::vec3(0.0f, 12.48f * 2.0f, 0.0f));

        carts = {
            Cart(glm::vec3(0.0f, 2.5958f * 2.0f, 0.045f * 2.0f),
                 glm::vec3(0.0f, -9.8242f * 2.0f, 0.045f * 2.0f)),
            Cart(glm::vec3(0.0f, 3.9378f * 2.0f, -4.8579f * 2.0f),
                 glm::vec3(0.0f, -8.5422f * 2.0f, -4.8579f * 2.0f)),
            Cart(glm::vec3(0.0f, 7.4953f * 2.0f, -8.4636f * 2.0f),
                 glm::vec3(0.0f, -4.9847f * 2.0f, -8.4636f * 2.0f)),
            Cart(glm::vec3(0.0f, 12.399f * 2.0f, -9.8097f * 2.0f),
                 glm::vec3(0.0f, -0.081f * 2.0f, -9.8097f * 2.0f)),
            Cart(glm::vec3(0.0f, 17.279f * 2.0f, -8.5838f * 2.0f),
                 glm::vec3(0.0f, 4.799f * 2.0f, -8.5838f * 2.0f)),
            Cart(glm::vec3(0.0f, 20.932f * 2.0f, -5.0262f * 2.0f),
                 glm::vec3(0.0f, 8.452f * 2.0f, -5.0262f * 2.0f)),
            Cart(glm::vec3(0.0f, 22.303f * 2.0f, -0.0504f * 2.0f),
                 glm::vec3(0.0f, 9.823f * 2.0f, -0.0504f * 2.0f)),
            Cart(glm::vec3(0.0f, 21.077f * 2.0f, 4.7812f * 2.0f),
                 glm::vec3(0.0f, 8.597f * 2.0f, 4.7812f * 2.0f)),
            Cart(glm::vec3(0.0f, 17.519f * 2.0f, 8.459f * 2.0f),
                 glm::vec3(0.0f, 5.039f * 2.0f, 8.459f * 2.0f)),
            Cart(glm::vec3(0.0f, 12.639f * 2.0f, 9.8051f * 2.0f),
                 glm::vec3(0.0f, 0.159f * 2.0f, 9.8051f * 2.0f)),
            Cart(glm::vec3(0.0f, 7.6646f * 2.0f, 8.5311f * 2.0f),
                 glm::vec3(0.0f, -4.8154f * 2.0f, 8.5311f * 2.0f)),
            Cart(glm::vec3(0.0f, 4.082f * 2.0f, 4.9735f * 2.0f),
                 glm::vec3(0.0f, -8.398f * 2.0f, 4.9735f * 2.0f)),
        };
        center = glm::vec3(0.0f, 12.48f * 2.0f, 0.0f);

        stand.translate(glm::vec3(38.0f, 0.0f, 20.0f));
        wheel.translate(glm::vec3(38.0f, 0.0f, 20.0f));
        for (Cart &cart : carts)
        {
            cart.cart.translate(glm::vec3(38.0f, 0.0f, 20.0f));
            cart.lamp->translate(glm::vec3(38.0f, 0.0f, 20.0f));
            cart.camera->Position += glm::vec3(38.0f, 0.0f, 20.0f);
        }

        booth = new TicketBooth(glm::vec3(30.0f, 0.0f, 15.0f));
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

    void draw(Shader &shader, bool depthOnly = false)
    {
        wheel.Draw(shader, depthOnly);
        stand.Draw(shader);
        for (auto &cart : carts)
        {
            cart.draw(shader, depthOnly);
        }
        booth->draw(shader);
    }

    void drawDepth(Shader &shader)
    {
        wheel.drawDepth(shader);
        stand.drawDepth(shader);
        for (auto &cart : carts)
        {
            cart.cart.drawDepth(shader);
        }
    }

    void translate(glm::vec3 translation)
    {
        stand.translate(translation);
        wheel.translate(translation);
        for (auto &cart : carts)
        {
            cart.cart.translate(translation);
            cart.updateCamera(translation);
        }
    }
};

#endif
