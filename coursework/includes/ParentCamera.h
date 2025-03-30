#ifndef PARENTCAMERA_H
#define PARENTCAMERA_H

#include "BoundingBox.h"
#include "CollisionManager.h"
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

    BoundingBox *boundingBox;
    CollisionManager *collisionManager;

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

    virtual void processInput(movement_dir dir, float deltaTime) = 0;
    virtual void ProcessMouseMovement(float xoffset, float yoffset) = 0;
    virtual glm::mat4 GetViewMatrix() = 0;

  private:
};

#endif
