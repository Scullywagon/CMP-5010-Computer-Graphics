#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "BoundingBox.h"
#include "ParentCamera.h"
#include <algorithm>
#include <iostream>
#include <vector>

class CollisionManager
{
  public:
    ParentCamera *player;

    std::vector<BoundingBox *> bbMinX;
    std::vector<BoundingBox *> bbMaxX;
    std::vector<BoundingBox *> bbMinY;
    std::vector<BoundingBox *> bbMaxY;

    float playerIndexMinX;
    float playerIndexMaxX;
    float playerIndexMinY;
    float playerIndexMaxY;

    void add(BoundingBox *a)
    {
        bbMinX.push_back(a);
        bbMaxX.push_back(a);
        bbMinY.push_back(a);
        bbMaxY.push_back(a);
    }

    void sortBound()
    {
        auto updateIndex = [](std::vector<BoundingBox *> &vec,
                              BoundingBox *playerBB) -> int
        {
            return std::distance(vec.begin(),
                                 std::find(vec.begin(), vec.end(), playerBB));
        };

        std::sort(bbMinX.begin(), bbMinX.end(),
                  [](const BoundingBox *a, const BoundingBox *b)
                  { return a->min.x < b->min.x; });

        std::sort(bbMaxX.begin(), bbMaxX.end(),
                  [](const BoundingBox *a, const BoundingBox *b)
                  { return a->max.x < b->max.x; });

        std::sort(bbMinY.begin(), bbMinY.end(),
                  [](const BoundingBox *a, const BoundingBox *b)
                  { return a->min.y < b->min.y; });

        std::sort(bbMaxY.begin(), bbMaxY.end(),
                  [](const BoundingBox *a, const BoundingBox *b)
                  { return a->max.y < b->max.y; });

        // Update player index after sorting
        playerIndexMinX = updateIndex(bbMinX, player->boundingBox);
        playerIndexMaxX = updateIndex(bbMaxX, player->boundingBox);
        playerIndexMinY = updateIndex(bbMinY, player->boundingBox);
        playerIndexMaxY = updateIndex(bbMaxY, player->boundingBox);
    }

    void addPlayer(ParentCamera *player)
    {
        // Add player bounding box to lists
        bbMinX.push_back(player->boundingBox);
        bbMaxX.push_back(player->boundingBox);
        bbMinY.push_back(player->boundingBox);
        bbMaxY.push_back(player->boundingBox);

        // Sort the bounding box lists
        sortBound();
    }

    void changePlayer(ParentCamera *camera)
    {
        bbMinX.erase(bbMinX.begin() + playerIndexMinX);
        bbMaxX.erase(bbMaxX.begin() + playerIndexMaxX);
        bbMinY.erase(bbMinY.begin() + playerIndexMinY);
        bbMaxY.erase(bbMaxY.begin() + playerIndexMaxY);
        addPlayer(camera);
    }

    void collidePlayer(BoundingBox *a)
    {
        player->translation = glm::vec3(0.0f);
    }

    void check()
    {
        std::cout << "player min x: " << playerIndexMinX
                  << " player max x: " << playerIndexMaxX << std::endl;
        for (int i = playerIndexMinX; i < playerIndexMaxX; i++)
        {
            checkCollision(bbMinX[i]);
            checkCollision(bbMaxX[i]);
        }
        for (int i = playerIndexMinY; i < playerIndexMaxY; i++)
        {
            checkCollision(bbMinY[i]);
            checkCollision(bbMaxY[i]);
        }
    }

  private:
    bool checkCollision(BoundingBox *a)
    {
        std::cout << "Checking collision" << std::endl;
        glm::vec3 translatedMin =
            player->boundingBox->min + player->translation;
        glm::vec3 translatedMax =
            player->boundingBox->max + player->translation;

        return !(translatedMax.x < a->min.x ||
                 translatedMin.x > a->max.x || // X-axis
                 translatedMax.y < a->min.y ||
                 translatedMin.y > a->max.y || // Y-axis
                 translatedMax.z < a->min.z ||
                 translatedMin.z > a->max.z); // Z-axis
    }
};

#endif
