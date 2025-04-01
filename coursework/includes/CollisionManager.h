#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "BoundingBox.h"
#include "ParentCamera.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

class CollisionManager
{
  public:
    // bounding box and Xmin,Xmax, Ymin,Ymax
    std::vector<BoundingBox *> items;

    ParentCamera *player;

    std::vector<BoundingBox *>
        boundingBoxesX; // sorted with two for min and max
    std::vector<BoundingBox *>
        boundingBoxesY; // sorted with two for min and max

    void add(BoundingBox *a)
    {
    }

    void addPlayer(ParentCamera *player)
    {
        this->player = player;
    }

    void init()
    {
        for (auto &item : items)
        {
            if (boundingBoxesX.size() == 0)
            {
                boundingBoxesX.push_back(item);
                boundingBoxesX.push_back(item);
                boundingBoxesY.push_back(item);
                boundingBoxesY.push_back(item);
                boundingBoxesX[0]->xmin = 0;
                boundingBoxesX[1]->xmax = 1;
                boundingBoxesY[0]->ymin = 0;
                boundingBoxesY[1]->ymax = 1;
                continue;
            }
            if (item->sorted == false)
            {
                insert(item);
            }
        }
    }

    void sortItems()
    {
        for (auto &item : items)
        {
            if (item->sorted == false)
            {
                boundingBoxesX.erase(boundingBoxesX.begin() + item->xmin);
                boundingBoxesX.erase(boundingBoxesX.begin() + item->xmax);
                boundingBoxesY.erase(boundingBoxesY.begin() + item->ymin);
                boundingBoxesY.erase(boundingBoxesY.begin() + item->ymax);
                insert(item);
            }
        }
        if (player->boundingBox->sorted == false)
        {
            boundingBoxesX.erase(boundingBoxesX.begin() +
                                 player->boundingBox->xmin);
            boundingBoxesX.erase(boundingBoxesX.begin() +
                                 player->boundingBox->xmax);
            boundingBoxesY.erase(boundingBoxesY.begin() +
                                 player->boundingBox->ymin);
            boundingBoxesY.erase(boundingBoxesY.begin() +
                                 player->boundingBox->ymax);
            insert(player->boundingBox);
        }
    }

    void insert(BoundingBox *item)
    {
        bool minIn = false, maxIn = false;
        for (int i = 0; i < boundingBoxesX.size(); i++)
        {
            if (item->min.x < boundingBoxesX[i]->min.x)
            {
                boundingBoxesX.insert(boundingBoxesX.begin() + i, item);
                boundingBoxesX[i]->xmin = i;
                boundingBoxesX[i + 1]->xmin = i + 1;
                minIn = true;
                continue;
            }
            if (item->max.x < boundingBoxesX[i]->max.x)
            {
                boundingBoxesX.insert(boundingBoxesX.begin() + i, item);
                boundingBoxesX[i]->xmax = i;
                boundingBoxesX[i + 1]->xmax = i + 1;
                maxIn = true;
                break;
            }
        }
        if (!minIn)
        {
            boundingBoxesX.push_back(item);
            boundingBoxesX.push_back(item);
            item->xmin = boundingBoxesX.size() - 2;
            item->xmax = boundingBoxesX.size() - 1;
        }
        else if (!maxIn)
        {
            boundingBoxesX.push_back(item);
            item->xmax = boundingBoxesX.size() - 1;
        }

        minIn = false, maxIn = false;
        for (int i = 0; i < boundingBoxesY.size(); i++)
        {
            if (item->min.y < boundingBoxesY[i]->min.y)
            {
                boundingBoxesY.insert(boundingBoxesY.begin() + i, item);
                boundingBoxesY[i]->ymin = i;
                boundingBoxesY[i + 1]->ymin = i + 1;
                minIn = true;
                continue;
            }
            if (item->max.y < boundingBoxesY[i]->max.y)
            {
                boundingBoxesY.insert(boundingBoxesY.begin() + i, item);
                boundingBoxesY[i]->ymax = i;
                boundingBoxesY[i + 1]->ymax = i + 1;
                maxIn = true;
                break;
            }
        }
        if (!minIn)
        {
            boundingBoxesX.push_back(item);
            boundingBoxesX.push_back(item);
            item->ymin = boundingBoxesY.size() - 2;
            item->ymax = boundingBoxesY.size() - 1;
        }
        else if (!maxIn)
        {
            boundingBoxesX.push_back(item);
            item->ymax = boundingBoxesY.size() - 1;
        }
        item->sorted = true;
    }

    void changePlayer(ParentCamera *player)
    {
        boundingBoxesX.erase(boundingBoxesX.begin() +
                             this->player->boundingBox->xmin);
        boundingBoxesX.erase(boundingBoxesX.begin() +
                             this->player->boundingBox->xmax);
        boundingBoxesY.erase(boundingBoxesY.begin() +
                             this->player->boundingBox->ymin);
        boundingBoxesY.erase(boundingBoxesY.begin() +
                             this->player->boundingBox->ymax);
        this->player = player;
        sortItems();
    }

    void collidePlayer(BoundingBox *a)
    {
        player->translation = glm::vec3(0.0f);
    }

    void check()
    {
        sortItems();
        int Pmin = player->boundingBox->xmin, Pmax = player->boundingBox->xmax;
        for (int i = Pmin + 1; i < Pmax; i++)
        {
            if (checkCollision(boundingBoxesX[i]))
            {
                collidePlayer(boundingBoxesX[i]);
            }
        }
        Pmin = player->boundingBox->ymin, Pmax = player->boundingBox->ymax;
        for (int i = Pmin + 1; i < Pmax; i++)
        {
            if (checkCollision(boundingBoxesY[i]))
            {
                collidePlayer(boundingBoxesY[i]);
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
