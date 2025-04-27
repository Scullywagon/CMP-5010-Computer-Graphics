#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "BoundingTree.h"
#include "Camera.h"
using namespace std;

class CollisionManager
{
  public:
    vector<pair<BoundingTree *, bool>> xList;
    vector<pair<BoundingTree *, bool>> zList;
    Camera *player;

    CollisionManager(Camera *p);
    void update();
    void genChecks();
    void add(BoundingTree *bt);    // for additions or reinsertions
    void checkNodes(BoundingNode *bn);  // checks each node and calls intself
    void isColliding(glm::vec3 pos);
    void collideWithVertex(glm::vec3 vert);

  private:
    static bool compareX(const std::pair<BoundingTree *, bool> &a,
                         const std::pair<BoundingTree *, bool> &b)
    {
        float valA = a.second == false ? a.first->min.x : a.first->max.x;
        float valB = b.second == false ? b.first->min.x : b.first->max.x;
        return valA < valB;
    }
    static bool compareZ(const std::pair<BoundingTree *, bool> &a,
                         const std::pair<BoundingTree *, bool> &b)
    {
        float valA = a.second == false ? a.first->min.z : a.first->max.z;
        float valB = b.second == false ? b.first->min.z : b.first->max.z;
        return valA < valB;
    }

    void sortItems()
    {
        std::sort(xList.begin(), xList.end(), compareX);
        std::sort(zList.begin(), zList.end(), compareZ);
    }

    bool shouldCheck(int i, vector<pair<BoundingTree *, bool>> &list);
};

#endif
