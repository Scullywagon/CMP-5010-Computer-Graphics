#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "ParentCamera.h"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrthographicCamera : public ParentCamera
{
  public:
    const float SPEED = 14.5f;
    const float FOV = 50.0f;

    float Left = -40.0f;
    float RightBound = 40.0f;
    float Bottom = -22.5f;
    float Top = 22.5f;
    float Near = -0.1f;
    float Far = 2000.0f;

    OrthographicCamera()
    {
        Position = glm::vec3(-100.0f, 100.0f, 100.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Right = glm::vec3(1.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        Yaw = YAW;
        Pitch = PITCH;

        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;
    }

    glm::mat4 GetViewMatrix() override
    {
        return glm::lookAt(Position, glm::vec3(0.0f), Up);
    }

    glm::mat4 GetProjectionMatrix() const
    {
        return glm::ortho(Left, RightBound, Bottom, Top, Near, Far);
    }

    void processInput(movement_dir dir, float deltaTime) override
    {
        float velocity = MovementSpeed * deltaTime;
        if (dir == LEFT)
        {
            glm::vec3 angle = glm::radians(glm::vec3(0.0f, -velocity, 0.0f));

            glm::quat rotation = glm::quat(angle);

            glm::vec3 rotatedPosition = rotation * Position;
            this->translation += rotatedPosition - Position;
        }
        if (dir == RIGHT)
        {
            glm::vec3 angle = glm::radians(glm::vec3(0.0f, velocity, 0.0f));

            glm::quat rotation = glm::quat(angle);

            glm::vec3 rotatedPosition = rotation * Position;
            this->translation += rotatedPosition - Position;
        }
        if (dir == UP)
        {
            float scale = 0.5f * velocity;

            if (Left <= -45.0f)
                return;

            Left -= scale;
            RightBound += scale;
            Bottom -= scale;
            Top += scale;
        }
        if (dir == DOWN)
        {
            float scale = -0.5f * velocity;

            if (Left >= -10.0f)
                return;

            Left -= scale;
            RightBound += scale;
            Bottom -= scale;
            Top += scale;
        }
        if (dir == BACKWARD)
        {
            translation += -WorldUp * velocity;
        }
        if (dir == FORWARD)
        {
            translation += WorldUp * velocity;
        }
    }

    void move() override
    {
        this->Position += translation;
        this->translation = glm::vec3(0.0f);
    }

    void ProcessMouseMovement(float xoffset, float yoffset) override
    {
    }

  private:
};
#endif
