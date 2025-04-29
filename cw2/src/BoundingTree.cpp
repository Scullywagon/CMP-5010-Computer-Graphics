#include "BoundingTree.h"
#include "glm/detail/func_geometric.hpp"
#include "glm/detail/type_vec.hpp"

BoundingNode::BoundingNode(glm::vec3 center, glm::vec3 front, glm::vec3 up,
                           glm::vec3 right, int index, Model &model)
{
    this->center = center;
    this->front = front;
    this->up = up;
    this->right = right;

    this->generateNodes(index, model);
}

void BoundingNode::generateNodes(int index, Model &model)
{
    if (index == 12)
    {
        bottom = true;
        return;
    }
    glm::vec3 newFront = this->front;
    glm::vec3 newUp = this->up;
    glm::vec3 newRight = this->right;
    glm::vec3 firstCenter = this->center;
    glm::vec3 secondCenter = this->center;

    if (index % 3 == 0)
    {
        newFront *= 0.5f;
        firstCenter += newFront;
        secondCenter -= newFront;
    }
    else if (index % 3 == 1)
    {
        newUp *= -0.5f;
        firstCenter += newUp;
        secondCenter -= newUp;
    }
    else if (index % 3 == 2)
    {
        newRight *= 0.5f;
        firstCenter += newRight;
        secondCenter -= newRight;
    }

    int newIndex = index + 1;

    if (newUp.length() < 0.5f && newRight.length() < 0.5f &&
        newFront.length() < 0.5f)
    {
        bottom = true;
        return;
    }

    first = new BoundingNode(firstCenter, newFront, newUp, newRight, newIndex,
                             model);
    second = new BoundingNode(secondCenter, newFront, newUp, newRight, newIndex,
                              model);
}

void BoundingNode::updatePos(glm::mat4 &modelMatrix)
{
    glm::vec4 newCenter = modelMatrix * glm::vec4(center, 1.0f);
    center = glm::vec3(newCenter.x, newCenter.y, newCenter.z);

    glm::vec4 newFront = modelMatrix * glm::vec4(front, 0.0f);
    front = glm::vec3(newFront.x, newFront.y, newFront.z);

    glm::vec4 newUp = modelMatrix * glm::vec4(up, 0.0f);
    up = glm::vec3(newUp.x, newUp.y, newUp.z);

    glm::vec4 newRight = modelMatrix * glm::vec4(right, 0.0f);
    right = glm::vec3(newRight.x, newRight.y, newRight.z);

    if (first != nullptr)
    {
        first->updatePos(modelMatrix);
    }
    if (second != nullptr)
    {
        second->updatePos(modelMatrix);
    }
}

void BoundingNode::translate(glm::vec3 &translation)
{
    center += translation;
    if (first != nullptr)
    {
        first->translate(translation);
    }
    if (second != nullptr)
    {
        second->translate(translation);
    }
}

// for player mainly
BoundingTree::BoundingTree(glm::vec3 position)
{
    this->max = position + glm::vec3(0.5f);
    this->max = position + glm::vec3(-0.5f);
}

BoundingTree::BoundingTree(Model &model, glm::mat4 *matrix)
{
    pair<glm::vec3, glm::vec3> box = genInitBox(model);
    min = box.first;
    max = box.second;
    this->model = &model;

    glm::vec3 center = (min + max) / 2.0f;
    glm::vec3 front(max.x - center.x, 0.0f, 0.0f);
    glm::vec3 up(0.0f, max.y - center.y, 0.0f);
    glm::vec3 right(0.0f, 0.0f, max.z - center.z);

    node = new BoundingNode(center, front, up, right, 0, model);
    assignColliders();
    updatePos(*matrix);
    generateAABB();
}

void BoundingTree::generateAABB()
{
    float xMax = std::abs(node->front.x) + std::abs(node->up.x) +
                 std::abs(node->right.x);
    float yMax = std::abs(node->front.y) + std::abs(node->up.y) +
                 std::abs(node->right.y);
    float zMax = std::abs(node->front.z) + std::abs(node->up.z) +
                 std::abs(node->right.z);

    min = node->center + glm::vec3(-xMax, -yMax, -zMax);
    max = node->center + glm::vec3(xMax, yMax, zMax);
}

void BoundingTree::translate(glm::vec3 trans)
{
    min += trans;
    max += trans;

    if (node != nullptr)
    {
    }
}

void BoundingTree::updatePos(glm::mat4 &modelMatrix)
{
    this->modelMatrix = &modelMatrix;
    if (node != nullptr)
        node->updatePos(modelMatrix);
    generateAABB();
}

pair<glm::vec3, glm::vec3> BoundingTree::genInitBox(Model &model)
{
    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());

    for (const Mesh &mesh : model.meshes)
    {
        for (Vertex vertex : mesh.vertices)
        {
            min = glm::min(min, vertex.Position);
            max = glm::max(max, vertex.Position);
        }
    }
    return pair<glm::vec3, glm::vec3>(min, max);
}

void BoundingTree::assignColliders()
{
    vector<BoundingNode *> bottomNodes;
    findBottomNodes(node, bottomNodes);

    if (bottomNodes.empty())
        return;

    glm::vec3 valVec =
        bottomNodes[0]->front + bottomNodes[0]->up + bottomNodes[0]->right;
    struct NodeBounds
    {
        BoundingNode *node;
        glm::vec3 min;
        glm::vec3 max;
    };

    vector<NodeBounds> activeNodes;
    activeNodes.reserve(bottomNodes.size());
    for (BoundingNode *node : bottomNodes)
    {
        NodeBounds nb;
        nb.node = node;
        nb.min = node->center - valVec;
        nb.max = node->center + valVec;
        activeNodes.push_back(nb);
    }

    for (const Mesh &mesh : model->meshes)
    {
#pragma omp parallel for
        for (int i = 0; i < mesh.indices.size(); i += 3)
        {

            const Vertex &p0 = mesh.vertices[mesh.indices[i]];
            const Vertex &p1 = mesh.vertices[mesh.indices[i + 1]];
            const Vertex &p2 = mesh.vertices[mesh.indices[i + 2]];

            glm::vec3 denom = p1.Position - p0.Position;

            // Check all active nodes against this triangle
#pragma omp parallel for
            for (NodeBounds &nb : activeNodes)
            {

                if (nb.node->collide)
                    continue; // already assigned, skip
                bool intersects = true;
                float tmin;
                float tmax;

                for (const glm::vec3 &pt :
                     {p0.Position, p1.Position, p2.Position})
                {
                    if (pt.x < nb.min.x && pt.x > nb.max.x && pt.y < nb.min.y &&
                        pt.y > nb.max.y && pt.z < nb.min.z && pt.z > nb.max.z)
                    {
                        nb.node->collide = true;
                        break;
                    }
                }

                if (nb.node->collide)
                    continue;

                for (int axis = 0; axis < 3; axis++)
                {
                    tmin = (nb.min[axis] - p0.Position[axis]) / denom[axis];
                    tmax = (nb.max[axis] - p0.Position[axis]) / denom[axis];
                    if (tmin > tmax)
                        std::swap(tmin, tmax);
                    if (tmin > 1.0f || tmax < 0.0f || tmax < tmin)
                    {
                        intersects = false;
                        break;
                    }
                }

                if (intersects)
                {
                    nb.node->collide = true;
                }
            }
        }
    }
}

void BoundingTree::findBottomNodes(BoundingNode *node,
                                   std::vector<BoundingNode *> &bottomNodes)
{
    if (node->bottom)
    {
        bottomNodes.push_back(node);
        return;
    }

    // Recursively check the left and right child nodes
    findBottomNodes(node->first, bottomNodes);
    findBottomNodes(node->second, bottomNodes);
}
