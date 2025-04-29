#ifndef PARENTCAMERA_H
#define PARENTCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ParentCamera
{
  public:
    enum movement_dir
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    const float YAW = -90.0f; // 90 aligns with the x-axis
    const float PITCH = 0.0f;
    const float SPEED = 4.5f;
    const float SENSITIVITY = 0.05f;
    const float FOV = 90.0f;
    const float floorHeight = 0.0f;
    const bool CONSTRAIN_PITCH = true;

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

    bool flight = false;

    glm::vec3 translation = glm::vec3(0.0f);

    virtual void processInput(movement_dir dir, float deltaTime) = 0;
    virtual void ProcessMouseMovement(float xoffset, float yoffset) = 0;
    virtual void move() = 0;
    virtual glm::mat4 GetViewMatrix() = 0;
    virtual glm::mat4 GetProjectionMatrix() const = 0;

  private:
};

#endif
