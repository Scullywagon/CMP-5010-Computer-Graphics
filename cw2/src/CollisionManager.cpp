#include "CollisionManager.h"
#include "BoundingTree.h"
#include "Model.h"
#include "glm/detail/func_geometric.hpp"
#include "glm/detail/type_vec.hpp"

CollisionManager::CollisionManager(Camera *p)
{
    this->player = p;
    add(p->bt);
}

void CollisionManager::update()
{
    sortItems();
    genChecks();
}

void CollisionManager::genChecks()
{
    unordered_set<BoundingTree *> checkX;
    unordered_set<BoundingTree *> checkZ;
    bool x = true;
    bool z = true;

    int pMaxIndex = 0;

#pragma omp parallel for
    for (int i = 0; i < xList.size() - 1; i++)
    {
        /*
        if (x == false && z == false)
            break;
        if (xList[i].first == player->bt)
            x = false;
        if (zList[i].first == player->bt)
            z = false;
         */

        if (xList[i].second == false && xList[i].first != player->bt &&
            shouldCheck(i, xList))
        {
            checkX.insert(xList[i].first);
        }
        if (zList[i].second == false && zList[i].first != player->bt &&
            shouldCheck(i, zList))
        {
            checkZ.insert(zList[i].first);
        }
    }

    for (BoundingTree *b : checkX)
    {
        if (checkZ.find(b) != checkZ.end())
        {
            checkNodes(b->node, b->modelMatrix, b->model);
        }
    }
}

void CollisionManager::checkNodes(BoundingNode *bn, glm::mat4 *modelMatrix,
                                  Model *model)
{
    bool colliding = isColliding(bn);
    if (!colliding)
        return;

    if (bn->bottom)
    {
        if (bn->collide)
        {
            glm::vec3 min = bn->center - bn->front - bn->up - bn->right;
            glm::vec3 max = bn->center + bn->front + bn->up + bn->right;
            collideWithPoly(min, max, player->Position);
        }
        return;
    }
    checkNodes(bn->first, modelMatrix, model);
    checkNodes(bn->second, modelMatrix, model);
}

bool CollisionManager::isColliding(BoundingNode *bn)
{
    float playerRadius = 0.5f;

    glm::vec3 pos = player->Position + player->translation;

    float playerProjF = glm::dot(pos - bn->center, glm::normalize(bn->front));

    float halfExtentFront = glm::length(bn->front);

    if (playerProjF < -(halfExtentFront + playerRadius) ||
        playerProjF > (halfExtentFront + playerRadius))
    {
        return false;
    }

    float playerProjU = glm::dot(pos - bn->center, glm::normalize(bn->up));

    float halfExtentUp = glm::length(bn->up);

    if (playerProjU < -(halfExtentUp + playerRadius) ||
        playerProjU > (halfExtentUp + playerRadius))
    {
        return false;
    }

    float playerProjR = glm::dot(pos - bn->center, glm::normalize(bn->right));

    float halfExtentRight = glm::length(bn->right);

    if (playerProjR < -(halfExtentRight + playerRadius) ||
        playerProjR > (halfExtentRight + playerRadius))
    {
        return false;
    }

    return true;
}

bool CollisionManager::isColliding(glm::vec3 min, glm::vec3 max,
                                   glm::vec3 playerPos)
{
    if (playerPos.x >= min.x && playerPos.x <= max.x && playerPos.y >= min.y &&
        playerPos.y <= max.y && playerPos.z >= min.z && playerPos.z <= max.z)
    {
        return true;
    }
    return false;
}

void CollisionManager::collideWithPoly(glm::vec3 min, glm::vec3 max,
                                       glm::vec3 playerPos)
{
    float playerRadius = 0.5f;

    glm::vec3 playerMin = playerPos - glm::vec3(playerRadius);
    glm::vec3 playerMax = playerPos + glm::vec3(playerRadius);

    glm::vec3 overlapMin = glm::max(min, playerMin);
    glm::vec3 overlapMax = glm::min(max, playerMax);

    if (overlapMin.x >= overlapMax.x || overlapMin.y >= overlapMax.y ||
        overlapMin.z >= overlapMax.z)
    {
        return;
    }

    glm::vec3 overlap = overlapMax - overlapMin;

    glm::vec3 correction(0.0f);
    if (overlap.x < overlap.y && overlap.x < overlap.z)
    {
        correction.x =
            overlap.x * (playerPos.x < (min.x + max.x) / 2.0f ? -1.0f : 1.0f);
    }
    else if (overlap.y < overlap.x && overlap.y < overlap.z)
    {
        correction.y =
            overlap.y * (playerPos.y < (min.y + max.y) / 2.0f ? -1.0f : 1.0f);
    }
    else
    {
        correction.z =
            overlap.z * (playerPos.z < (min.z + max.z) / 2.0f ? -1.0f : 1.0f);
    }

    player->translation += correction;
}

void CollisionManager::add(BoundingTree *bt)
{
    std::pair<BoundingTree *, bool> min(bt, false);
    std::pair<BoundingTree *, bool> max(bt, true);

    xList.push_back(min);
    xList.push_back(max);
    zList.push_back(min);
    zList.push_back(max);
}

bool CollisionManager::shouldCheck(int i,
                                   vector<pair<BoundingTree *, bool>> &list)
{
    BoundingTree *bt = list[i].first;
    i++;
    while (list[i].first != bt && i < list.size())
    {
        if (list[i].first == player->bt)
        {
            return true;
        }
        i++;
    }
    return false;
}
