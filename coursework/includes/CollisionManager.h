#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "BoundingBox.h"
#include <iostream>
#include <vector>

class CollisionManager
{
  public:
    std::vector<BoundingBox *> boundingBoxes;
    BoundingBox *player;

    void listBounds()
    {
        for (auto &a : boundingBoxes)
        {
        }
    }

    void collidePlayer(BoundingBox *a)
    {
    }

    void check()
    {
        for (auto &a : boundingBoxes)
        {
            if (checkCollision(a))
            {
                std::cout << "Collision Detected" << std::endl;
                collidePlayer(a);
            }
        }
    }

  private:
    bool checkCollision(BoundingBox *a)
    {
        return !(
            player->max.x < a->min.x || player->min.x > a->max.x || // X-axis
            player->max.y < a->min.y || player->min.y > a->max.y || // Y-axis
            player->max.z < a->min.z || player->min.z > a->max.z);  // Z-axis
    }
};

#endif
