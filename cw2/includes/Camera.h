#ifndef CAMERA_H
#define CAMERA_H

#include "BoundingTree.h"
#include "Constants.h"
#include "ParentCamera.h"
#include "glm/detail/func_trigonometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

using namespace std;

// for players
class Camera : public ParentCamera
{
  public:
    const float SPEED = 14.5f;
    const float FOV = 50.0f;
    BoundingTree *bt;

    Camera()
    {
        // Initialize the base class members directly
        Position = glm::vec3(-50.0f, 10.0f, 0.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Right = glm::vec3(1.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        Yaw = YAW;
        Pitch = PITCH;

        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;

        fov = glm::radians(FOV);
        bt = new BoundingTree(Position);
    }

    glm::mat4 GetViewMatrix() override
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix() const override
    {
        return glm::perspective(fov,
                                (float)Constants::SCREEN_WIDTH /
                                    (float)Constants::SCREEN_HEIGHT,
                                0.1f, 100.0f);
    }

    void processInput(movement_dir dir, float deltaTime) override
    {
        float velocity = MovementSpeed * deltaTime;

        if (dir == FORWARD)
        {
            this->translation += Front * velocity;
        }
        if (dir == BACKWARD)
        {
            this->translation += -Front * velocity;
        }
        if (dir == LEFT)
        {
            this->translation += -Right * velocity;
        }
        if (dir == RIGHT)
        {
            this->translation += Right * velocity;
        }
        if (flight)
        {
            if (dir == UP)
            {
                this->translation += Up * velocity;
            }
            if (dir == DOWN)
            {
                this->translation += -Up * velocity;
            }
        }
    }

    void move() override
    {
        this->Position += translation;
        bt->translate(translation);
        float transY = 0.0f;

        if (Position.y <= (floorHeight + 0.5f))
        {
            transY = floorHeight + 0.5f - Position.y;
            Position.y = floorHeight + 0.5f;
        }
        else if (!flight)
        {
            transY = 0.0f;
            Position.y = floorHeight + 1.8f;
        }
        translation = glm::vec3(0.0f);
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

#endif
