#ifndef BOUNDINGTREE_H
#define BOUNDINGTREE_H

#include "Model.h"
#include <utility>
#include <limits>
#include "glm/detail/type_vec.hpp"
#include <glm/glm.hpp>
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
    vector<pair<int, int>> indexes; // holds indexes of indecies that make a triangle

    bool bottom = false;

    BoundingNode(glm::vec3 center, glm::vec3 front, glm::vec3 up,
                 glm::vec3 right, int index, Model &model);
    void generateNodes(int index, Model &model);
    void collectIndexes(Model &model);
};

class BoundingTree
{
  public:
    BoundingNode *node;
    glm::vec3 min;
    glm::vec3 max;

    BoundingTree(Model &model);
    BoundingTree(glm::vec3 position);
    void generateAABB();
    void translate(glm::vec3 trans);

  private:
    pair<glm::vec3, glm::vec3> genInitBox(Model &model);
};

#endif
