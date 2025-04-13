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
    // 1: boundingBox*  2: isMax
    std::vector<std::pair<BoundingBox *, bool>> itemsX;
    std::vector<std::pair<BoundingBox *, bool>> itemsZ;

    ParentCamera *player;

    void add(BoundingBox *a)
    {
        std::pair<BoundingBox *, bool> min(a, false);
        std::pair<BoundingBox *, bool> max(a, true);

        itemsX.push_back(min);
        itemsX.push_back(max);
        itemsZ.push_back(min);
        itemsZ.push_back(max);
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
    static bool compareZ(const std::pair<BoundingBox *, bool> &a,
                         const std::pair<BoundingBox *, bool> &b)
    {
        float valA = a.second == false ? a.first->min.z : a.first->max.z;
        float valB = b.second == false ? b.first->min.z : b.first->max.z;
        return valA < valB;
    }

    void sortItems()
    {
        std::sort(itemsX.begin(), itemsX.end(), compareX);
        std::sort(itemsZ.begin(), itemsZ.end(), compareZ);
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
        for (int i = 0; i < itemsZ.size(); i++)
        {
            if (itemsZ[i].first == this->player->boundingBox)
            {
                itemsZ[i].first = player->boundingBox;
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
                player->translation += calculateCorrection(x);
            }
        }

        // player->translation += totalCorrection;
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
        std::unordered_set<BoundingBox *> setZ = genSet(itemsZ);

        std::vector<BoundingBox *> subBoxes;

        for (BoundingBox *a : setX)
        {
            if (setZ.find(a) != setZ.end())
            {
                if (checkCollision(a))
                {
                    if (!a->checkSubBoxes)
                    {
                        subBoxes.insert(subBoxes.begin(), a);
                        continue;
                    }
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
        //std::cout << "Checking collision" << std::endl;
        glm::vec3 translatedMin =
            player->boundingBox->min + player->translation;
        glm::vec3 translatedMax =
            player->boundingBox->max + player->translation;

        return !(translatedMax.x < a->min.x || translatedMin.x > a->max.x ||
                 translatedMax.y < a->min.y || translatedMin.y > a->max.y ||
                 translatedMax.z < a->min.z || translatedMin.z > a->max.z);
    }

    glm::vec3 calcOverlap(BoundingBox *bb)
    {
        glm::vec3 translatedMin =
            player->boundingBox->min + player->translation;
        glm::vec3 translatedMax =
            player->boundingBox->max + player->translation;

        glm::vec3 overlapMin = glm::max(translatedMin, bb->min);
        glm::vec3 overlapMax = glm::min(translatedMax, bb->max);

        glm::vec3 overlap = overlapMax - overlapMin;

        if (overlap.x <= 0 || overlap.y <= 0 || overlap.z <= 0)
        {
            return glm::vec3(0.0f);
        }

        return overlap;
    }

    glm::vec3 calculateCorrection(BoundingBox *bb)
    {
        glm::vec3 correction(0.0f);
        glm::vec3 boxCenter = (bb->min + bb->max) / 2.0f;
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
