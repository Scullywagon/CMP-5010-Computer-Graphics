#ifndef PERSONCAMERA_H
#define PERSONCAMERA_H

#include "CollisionManager.h"
#include "ParentCamera.h"
#include "glm/detail/func_trigonometric.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PersonCamera : public ParentCamera
{
  public:
    PersonCamera()
    {
        // Initialize the base class members directly
        Position = glm::vec3(10.0f, 1.8f, 10.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Right = glm::vec3(1.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        Yaw = YAW;
        Pitch = PITCH;

        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;

        fov = glm::radians(FOV);

        boundingBox = new BoundingBox(Position - glm::vec3(0.5, 0.5, 0.5),
                                      Position + glm::vec3(0.5, 0.5, 0.5));
    }

    glm::mat4 GetViewMatrix() override
    {
        return glm::lookAt(Position, Position + Front, Up);
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
    }

    void move() override
    {
        translation.y = 0.0f;
        this->Position += translation;
        boundingBox->translate(translation);

        translation = glm::vec3(0.0f);
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

#endif
