#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "BoundingBox.h"
#include "ParentCamera.h"
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
        itemsX.push_back(min);
        std::pair<BoundingBox *, bool> max(a, true);
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

    void collidePlayer(BoundingBox *a)
    {
        player->translation = glm::vec3(0.0f);
    }

    std::unordered_set<BoundingBox *> genSet(std::vector<std::pair<BoundingBox *, bool>> &items)
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
                    if (items[x].second == true)
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
        std::unordered_set<BoundingBox *> setZ = genSet(itemsZ); // Changed Y to Z

        for (BoundingBox *a : setZ)
        {
            if (checkCollision(a))
            {
                collidePlayer(a);
            }
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
