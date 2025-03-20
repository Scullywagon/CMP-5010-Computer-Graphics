#ifndef CAMERA_VALUES_H
#define CAMERA_VALUES_H

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

#endif
