#ifndef BOUNDINGTREE_H
#define BOUNDINGTREE_H

#include "Model.h"
#include "glm/detail/type_vec.hpp"
#include <glm/glm.hpp>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

struct BoundingNode
{
    glm::vec3 center;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    BoundingNode *first;
    BoundingNode *second;
    vector<pair<int, int>>
        indexes; // holds indexes of indecies that make a triangle

    bool bottom = false;
    bool collide = false;

    BoundingNode(glm::vec3 center, glm::vec3 front, glm::vec3 up,
                 glm::vec3 right, int index, Model &model);
    void generateNodes(int index, Model &model);
    void collectIndexes(Model &model);
};

class BoundingTree
{
  public:
    BoundingNode *node;
    glm::mat4 *modelMatrix;
    glm::vec3 min;
    glm::vec3 max;
    Model *model;

    BoundingTree(Model &model, glm::mat4 *matrix);
    BoundingTree(glm::vec3 position);
    void generateAABB();
    void translate(glm::vec3 trans);

  private:
    pair<glm::vec3, glm::vec3> genInitBox(Model &model);
    void assignColliders();
    void findBottomNodes(BoundingNode *node,
                         std::vector<BoundingNode *> &bottomNodes);
};

#endif
