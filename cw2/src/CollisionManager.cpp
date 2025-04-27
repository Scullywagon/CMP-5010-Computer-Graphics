#include "CollisionManager.h"
#include "BoundingTree.h"
#include "Model.h"
#include "glm/detail/func_geometric.hpp"

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

    // #pragma omp parallel for
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
    if (!isColliding(bn))
        return;

    glm::mat4 inverse = glm::inverse(*modelMatrix);
    if (bn->bottom)
    {
        for (pair<int, int> id : bn->indexes)
        {
            glm::vec3 playerPos = glm::vec3(
                inverse *
                glm::vec4(player->Position + player->translation, 1.0f));
            glm::vec3 vert =
                model->meshes[id.first].vertices[id.second].Position;
            if (isColliding(vert, playerPos))
            {
                collideWithVertex(model->meshes[id.first].vertices[id.second],
                                  playerPos, inverse);
            }
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

bool CollisionManager::isColliding(glm::vec3 pos, glm::vec3 playerPos)
{
    if (glm::length(pos - playerPos) < 0.5f)
    {
        cout << "collided with: " << pos.x << " " << pos.y << " " << pos.z
             << endl;
        return true;
    }
    return false;
}

void CollisionManager::collideWithVertex(Vertex v, glm::vec3 playerPos,
                                         glm::mat4 inverse)
{
    glm::vec3 delta = playerPos - v.Position;
    float distance = glm::length(delta);

    float overlap = 0.5f - distance;
    if (overlap > 0.0f)
    {
        glm::vec3 correctionDir = glm::normalize(delta);
        glm::vec3 correction = correctionDir * overlap;

        glm::vec3 worldCorrection =
            glm::vec3(glm::inverse(inverse) * glm::vec4(correction, 0.0f));

        player->translation += worldCorrection;
    }
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
