#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "BoundingBox.h"
#include "ParentCamera.h"
#include "glm/detail/type_vec.hpp"
#include <algorithm>
#include <iostream>
#include <locale>
#include <map>
#include <unordered_set>
#include <utility>
#include <vector>

class CollisionManager
{
  public:
    // bounding box and isMax
    std::vector<std::pair<BoundingBox *, bool>> itemsX;
    std::vector<std::pair<BoundingBox *, bool>> itemsZ; // Changed Y to Z

    ParentCamera *player;

    void add(BoundingBox *a)
    {
        std::pair<BoundingBox *, bool> min(a, false);
        std::pair<BoundingBox *, bool> max(a, true);

        itemsX.push_back(min);
        itemsX.push_back(max);
        itemsZ.push_back(min); // Changed Y to Z
        itemsZ.push_back(max); // Changed Y to Z
    }

    void addPlayer(ParentCamera *player)
    {
        this->player = player;
        add(player->boundingBox);
    }

    static bool compareX(const std::pair<BoundingBox *, bool> &a,
                         const std::pair<BoundingBox *, bool> &b)
    {
        float valA = a.second == false ? a.first->min.x : a.first->max.x;
        float valB = b.second == false ? b.first->min.x : b.first->max.x;
        return valA < valB;
    }
    static bool
    compareZ(const std::pair<BoundingBox *, bool> &a, // Changed Y to Z
             const std::pair<BoundingBox *, bool> &b) // Changed Y to Z
    {
        float valA = a.second == false ? a.first->min.z
                                       : a.first->max.z; // Changed Y to Z
        float valB = b.second == false ? b.first->min.z
                                       : b.first->max.z; // Changed Y to Z
        return valA < valB;
    }

    void sortItems()
    {
        std::sort(itemsX.begin(), itemsX.end(), compareX);
        std::sort(itemsZ.begin(), itemsZ.end(), compareZ); // Changed Y to Z
    }

    void changePlayer(ParentCamera *player)
    {
        for (int i = 0; i < itemsX.size(); i++)
        {
            if (itemsX[i].first == this->player->boundingBox)
            {
                itemsX[i].first = player->boundingBox;
            }
        }
        for (int i = 0; i < itemsZ.size(); i++) // Changed Y to Z
        {
            if (itemsZ[i].first == this->player->boundingBox) // Changed Y to Z
            {
                itemsZ[i].first = player->boundingBox; // Changed Y to Z
            }
        }
        this->player = player;
        sortItems();
    }

    void collidePlayer(std::vector<BoundingBox *> &subBoxes)
    {
        glm::vec3 totalCorrection(0.0f);

        for (BoundingBox *x : subBoxes)
        {
            if (player->boundingBox->isColliding(*x))
            {
                totalCorrection += calculateCorrection(x);
            }
        }

        player->translation += totalCorrection;
    }

    std::unordered_set<BoundingBox *>
    genSet(std::vector<std::pair<BoundingBox *, bool>> &items)
    {
        std::unordered_set<BoundingBox *> set;
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].first == player->boundingBox)
            {
                if (items[i].second == true)
                {
                    break;
                }
                continue;
            }
            if (items[i].second == false)
            {
                for (int x = i; x < items.size(); x++)
                {
                    if (items[x].first == player->boundingBox)
                    {
                        set.insert(items[i].first);
                        break;
                    }
                    if (items[x].first == items[i].first &&
                        items[x].second == true)
                    {
                        break;
                    }
                }
            }
        }
        return set;
    }

    void check()
    {
        sortItems();
        std::unordered_set<BoundingBox *> setX = genSet(itemsX);
        std::unordered_set<BoundingBox *> setZ =
            genSet(itemsZ); // Changed Y to Z

        std::vector<BoundingBox *> subBoxes;

        for (BoundingBox *a : setX)
        {
            if (setZ.find(a) != setZ.end())
            {
                if (checkCollision(a))
                {
                    for (BoundingBox *x : a->subBoxes)
                    {
                        subBoxes.push_back(x);
                    }
                }
            }
        }
        collidePlayer(subBoxes);
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

    ///////
    /// CHANGE THESE LATER PLEASE
    ///////

    glm::vec3 calcOverlap(BoundingBox *bb)
    {
        glm::vec3 translatedMin =
            player->boundingBox->min + player->translation;
        glm::vec3 translatedMax =
            player->boundingBox->max + player->translation;

        // Calculate the overlap on each axis
        glm::vec3 overlapMin = glm::max(translatedMin, bb->min);
        glm::vec3 overlapMax = glm::min(translatedMax, bb->max);

        // Compute the overlap vector (the difference between max and min
        // overlap)
        glm::vec3 overlap = overlapMax - overlapMin;

        // If any overlap is negative or zero, there's no actual overlap
        if (overlap.x <= 0 || overlap.y <= 0 || overlap.z <= 0)
        {
            // No overlap along any axis, return a zero vector
            return glm::vec3(0.0f);
        }

        return overlap; // Return the overlap vector
    }

    glm::vec3 calcPushDirection(BoundingBox *bb)
    {
        // Get the overlap vector
        glm::vec3 overlap = calcOverlap(bb);

        // If there's no overlap, return a zero vector (no push required)
        if (overlap == glm::vec3(0.0f))
        {
            return glm::vec3(0.0f);
        }

        glm::vec3 translatedMin =
            player->boundingBox->min + player->translation;
        glm::vec3 translatedMax =
            player->boundingBox->max + player->translation;

        // Calculate the push direction based on the overlap
        glm::vec3 pushDirection(0.0f);

        if (overlap.x > 0)
        {
            // If the overlap on the X axis is positive, resolve it
            if (translatedMin.x < bb->min.x)
            {
                pushDirection.x = -overlap.x; // Move the player to the left
            }
            else
            {
                pushDirection.x = overlap.x; // Move the player to the right
            }
        }

        if (overlap.y > 0)
        {
            // If the overlap on the Y axis is positive, resolve it
            if (translatedMin.y < bb->min.y)
            {
                pushDirection.y = -overlap.y; // Move the player down
            }
            else
            {
                pushDirection.y = overlap.y; // Move the player up
            }
        }

        if (overlap.z > 0)
        {
            // If the overlap on the Z axis is positive, resolve it
            if (translatedMin.z < bb->min.z)
            {
                pushDirection.z = -overlap.z; // Move the player backward
            }
            else
            {
                pushDirection.z = overlap.z; // Move the player forward
            }
        }

        return pushDirection; // Return the calculated push direction
    }

    glm::vec3 calculateCorrection(BoundingBox *bb)
    {
        glm::vec3 correction(0.0f);
        glm::vec3 boxCenter =
            (bb->min + bb->max) / 2.0f; // Center of the bounding box
        float xSign = (player->Position.x < boxCenter.x) ? -1.0f : 1.0f;
        float ySign = (player->Position.y < boxCenter.y) ? -1.0f : 1.0f;
        float zSign = (player->Position.z < boxCenter.z) ? -1.0f : 1.0f;

        glm::vec3 overlap = calcOverlap(bb);

        if (overlap.x <= overlap.y && overlap.x <= overlap.z)
        {
            correction.x = xSign * overlap.x;
        }
        else if (overlap.y <= overlap.x && overlap.y <= overlap.z)
        {
            correction.y = ySign * overlap.y;
        }
        else
        {
            correction.z = zSign * overlap.z;
        }

        return correction;
    }
};

#endif
