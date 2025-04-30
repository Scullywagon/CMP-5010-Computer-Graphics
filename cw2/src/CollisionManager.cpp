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

    for (int i = 0; i < o->node.size(); i++)
    {
        bool colliding = isColliding(&o->node[i]);
        if (!colliding)
            continue;
        cout << "colliding with node :" << i << endl;
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
    glm::vec3 rel = pos - n->center;

    float projF = glm::dot(rel, n->parent->f);
    float projU = glm::dot(rel, n->parent->u);
    float projR = glm::dot(rel, n->parent->r);

    if (std::abs(projF) > n->front + playerRadius)
        return false;
    if (std::abs(projU) > n->up + playerRadius)
        return false;
    if (std::abs(projR) > n->right + playerRadius)
        return false;
    return true;
}
// change

void CollisionManager::collideWithNode(OctNode *n)
{
    float playerRadius = 0.5f;
    glm::vec3 pos = player->Position + player->translation;
    glm::vec3 rel = pos - n->center;

    float projF = glm::dot(rel, n->parent->f);
    float projU = glm::dot(rel, n->parent->u);
    float projR = glm::dot(rel, n->parent->r);

    float distF = std::abs(projF) - (n->front + playerRadius);
    float distU = std::abs(projU) - (n->up + playerRadius);
    float distR = std::abs(projR) - (n->right + playerRadius);

    if (distF < 0.0f && distU < 0.0f && distR < 0.0f)
    {
        if (distF > distU && distF > distR)
        {
            float overlap = -distF;
            player->translation += glm::sign(projF) * n->parent->f * overlap;
        }
        else if (distU > distR)
        {
            float overlap = -distU;
            player->translation += glm::sign(projU) * n->parent->u * overlap;
        }
        else
        {
            float overlap = -distR;
            player->translation += glm::sign(projR) * n->parent->r * overlap;
        }
    }
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
