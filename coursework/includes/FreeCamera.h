#include "CameraValues.h"
#include "glm/detail/func_trigonometric.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FreeCamera
{
  public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;

    float fov; // in radians

    FreeCamera()
    {
        Position = glm::vec3(0.0f, 1.8f, 0.0f);
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

    // this method returns the view matrix calculated using Euler Angles
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void processInput(movement_dir dir, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;

        if (dir == FORWARD)
        {
            Position += Front * velocity;
        }
        if (dir == BACKWARD)
        {
            Position -= Front * velocity;
        }
        if (dir == LEFT)
        {
            Position -= Right * velocity;
        }
        if (dir == RIGHT)
        {
            Position += Right * velocity;
        }

        Position.y = floorHeight + 1.8f;
    }

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        // change in mouse pos combined with sensitivity
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        // camera yaw and pitch changed
        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get
        // flipped
        if (CONSTRAIN_PITCH)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -77.0f)
                Pitch = -77.0f;
        }
        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

  private:
    void updateCameraVectors()
    {
        // calculate the new Front vector as the front is now a vector in a new
        // dir
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(
            Front, WorldUp)); // normalize the vectors, because their length
                              // gets closer to 0 the more you look up or down
                              // which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
