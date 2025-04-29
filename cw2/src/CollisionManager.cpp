#include "CollisionManager.h"
#include "BoundingTree.h"
#include "Model.h"
#include "Octree.h"
#include "glm/detail/func_geometric.hpp"
#include "glm/detail/type_vec.hpp"

CollisionManager::CollisionManager(Camera *p)
{
    this->player = p;
}

void CollisionManager::init()
{
    sortItems();
}

void CollisionManager::update()
{
    sortItems();
    genChecks();
}

void CollisionManager::genChecks()
{
    unordered_set<Octree *> checkX;
    unordered_set<Octree *> checkZ;
    bool x = true;
    bool z = true;

    int pMaxIndex = 0;

    for (int i = 0; i < xList.size() - 1; i++)
    {
        if (!xList[i].second && xList[i].first != nullptr &&
            std::abs(xList[i].first->center.x - player->Position.x) <
                (xList[i].first->radius + 0.5))
        {
            checkX.insert(xList[i].first);
        }
        if (!zList[i].second && zList[i].first != nullptr &&
            std::abs(zList[i].first->center.z - player->Position.z) <
                (zList[i].first->radius + 0.5))
        {
            checkZ.insert(zList[i].first);
        }
    }

    for (Octree *o : checkX)
    {
        if (checkZ.find(o) != checkZ.end())
        {
            checkNodes(o);
        }
    }
}

void CollisionManager::checkNodes(Octree *o)
{
    if (!isColliding(o))
        return;

    for (int i = 0; i < 8; i++)
    {
        bool colliding = isColliding(&o->node[i]);
        if (!colliding)
            continue;
        collideWithNode(&o->node[i]);
    }
}

bool CollisionManager::isColliding(Octree *o)
{
    float playerRadius = 0.5f;
    glm::vec3 pos = player->Position + player->translation;
    glm::vec3 rel = pos - o->center;

    float projF = glm::dot(rel, o->f);
    float projU = glm::dot(rel, o->u);
    float projR = glm::dot(rel, o->r);

    if (std::abs(projF) > o->front + playerRadius)
        return false;
    if (std::abs(projU) > o->up + playerRadius)
        return false;
    if (std::abs(projR) > o->right + playerRadius)
        return false;
    return true;
}

bool CollisionManager::isColliding(OctNode *n)
{
    float playerRadius = 0.5f;

    glm::vec3 pos = player->Position + player->translation;

    float playerProjF = glm::dot(pos - n->center, n->parent->f);

    if (playerProjF < -(n->front + playerRadius) ||
        playerProjF > (n->front + playerRadius))
    {
        return false;
    }

    float playerProjU = glm::dot(pos - n->center, n->parent->u);
    if (playerProjU < -(n->up + playerRadius) ||
        playerProjU > (n->up + playerRadius))
    {
        return false;
    }

    float playerProjR = glm::dot(pos - n->center, n->parent->r);
    if (playerProjR < -(n->right + playerRadius) ||
        playerProjR > (n->right + playerRadius))
    {
        return false;
    }

    return true;
}
// change
void CollisionManager::collideWithNode(OctNode *n)
{
    float playerRadius = 0.5f;
    glm::vec3 pos = player->Position + player->translation;

    glm::vec3 diff = pos - n->center;

    float playerProjF = glm::dot(diff, n->parent->f);
    float overlapF = (n->front + playerRadius) - std::abs(playerProjF);
    if (overlapF <= 0)
        return;

    float playerProjU = glm::dot(diff, n->parent->u);
    float overlapU = (n->up + playerRadius) - std::abs(playerProjU);
    if (overlapU <= 0)
        return;

    float playerProjR = glm::dot(diff, n->parent->r);
    float overlapR = (n->right + playerRadius) - std::abs(playerProjR);
    if (overlapR <= 0)
        return;

    // Find the axis with the minimum penetration
    float minOverlap = std::min({overlapF, overlapU, overlapR});
    glm::vec3 pushDir;

    if (minOverlap == overlapF)
        pushDir = n->parent->f * (playerProjF < 0 ? 1.0f : -1.0f) * overlapF;
    else if (minOverlap == overlapU)
        pushDir = n->parent->u * (playerProjU < 0 ? 1.0f : -1.0f) * overlapU;
    else
        pushDir = n->parent->r * (playerProjR < 0 ? 1.0f : -1.0f) * overlapR;

    player->translation += pushDir;
}

void CollisionManager::add(Octree *o)
{
    std::pair<Octree *, bool> min(o, false);
    std::pair<Octree *, bool> max(o, true);

    xList.push_back(min);
    xList.push_back(max);
    zList.push_back(min);
    zList.push_back(max);
}
