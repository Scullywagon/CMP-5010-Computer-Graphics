#include "CollisionManager.h"
#include "BoundingTree.h"

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
    for (int i = 0; i < xList.size() - 1; i++)
    {
        if (x == false && z == false)
            break;
        if (xList[i].first == player->bt)
            x = false;
        if (zList[i].first == player->bt)
            z = false;

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

    #pragma omp parallel for
    for (BoundingTree *b : checkX)
    {
        if (checkZ.find(b) != checkZ.end())
        {
            checkNodes(b->node);
        }
    }
}

void CollisionManager::checkNodes(BoundingNode *bn)
{
    if (bn->bottom)
    {
        for (pair<int, int> id : bn->indexes)
        {
        }
        return;
    }
    checkNodes(bn->first);
    checkNodes(bn->second);
}

void CollisionManager::isColliding(glm::vec3 pos)
{
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
