#ifndef OCTREE_H
#define OCTREE_H
#include "Model.h"
#include <glm/glm.hpp>

class Octree;

struct OctNode
{
    Octree *parent;
    float front, up, right;
    glm::vec3 center;

    OctNode() = default;
    void generate(vector<glm::vec3> &positions);
};

class Octree
{
  public:
    OctNode node[8];
    glm::vec3 min, max, center;
    float front, up, right, radius;
    glm::vec3 f = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 r = glm::vec3(0.0f, 0.0f, 1.0f);
    Octree(Model *model, glm::mat4 modelMatrix);
    void updatePos(glm::mat4 modelMatrix);

  private:
    vector<glm::vec3> getPositions(Model *model);
};

#endif
