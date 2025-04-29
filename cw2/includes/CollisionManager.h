#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "BoundingTree.h"
#include "Camera.h"
#include "Octree.h"
using namespace std;

class CollisionManager
{
  public:
    vector<pair<Octree *, bool>> xList;
    vector<pair<Octree *, bool>> zList;
    Camera *player;

    CollisionManager(Camera *p);
    void init();
    void update();
    void genChecks();
    void add(Octree *o);
    void checkNodes(Octree *o); // checks each node and calls intself
    bool isColliding(Octree *o);
    bool isColliding(OctNode *n);
    void collideWithNode(OctNode *n);

  private:
    static bool compareX(const std::pair<Octree *, bool> &a,
                         const std::pair<Octree *, bool> &b)
    {
        float valA = a.second == false ? a.first->center.x - a.first->radius
                                       : a.first->center.x + a.first->radius;
        float valB = b.second == false ? b.first->center.x - b.first->radius
                                       : b.first->center.x + b.first->radius;
        return valA < valB;
    }

    static bool compareZ(const std::pair<Octree *, bool> &a,
                         const std::pair<Octree *, bool> &b)
    {
        float valA = a.second == false ? a.first->center.z - a.first->radius
                                       : a.first->center.z + a.first->radius;
        float valB = b.second == false ? b.first->center.z - b.first->radius
                                       : b.first->center.z + b.first->radius;
        return valA < valB;
    }

    void sortItems()
    {
        std::sort(xList.begin(), xList.end(), compareX);
        std::sort(zList.begin(), zList.end(), compareZ);
    }

    bool shouldCheck(int i, vector<pair<Octree *, bool>> &list);
};

#endif
