#include "BoundingTree.h"
#include "glm/detail/type_vec.hpp"

BoundingNode::BoundingNode(glm::vec3 center, glm::vec3 front, glm::vec3 up,
                           glm::vec3 right, int index, Model &model)
{
    this->front = front;
    this->up = up;
    this->right = right;

    this->generateNodes(index, model);
}

void BoundingNode::generateNodes(int index, Model &model)
{
    if (index == 3)
    {
        bottom = true;
        collectIndexes(model);
        return;
    }

    index++;

    float frontLen = glm::length(front);
    float upLen = glm::length(up);
    float rightLen = glm::length(right);

    glm::vec3 splitAxis;
    glm::vec3 newFront = front;
    glm::vec3 newUp = up;
    glm::vec3 newRight = right;

    if (frontLen >= upLen && frontLen >= rightLen)
    {
        splitAxis = front;
        newFront = 0.5f * front; // halve the split axis
    }
    else if (upLen >= frontLen && upLen >= rightLen)
    {
        splitAxis = up;
        newUp = 0.5f * up;
    }
    else
    {
        splitAxis = right;
        newRight = 0.5f * right;
    }

    glm::vec3 halfVec = 0.5f * splitAxis;

    glm::vec3 newCenter = center + halfVec;
    first =
        new BoundingNode(newCenter, newFront, newUp, newRight, index, model);

    newCenter = center - halfVec;
    second =
        new BoundingNode(newCenter, newFront, newUp, newRight, index, model);
}

void BoundingNode::collectIndexes(Model& model)
{
    indexes.clear();

    for (int meshIndex = 0; meshIndex < model.meshes.size(); ++meshIndex)
    {
        Mesh& mesh = model.meshes[meshIndex];
        for (int v = 0; v < mesh.vertices.size(); ++v)
        {
            glm::vec3 pos = mesh.vertices[v].Position - center; // Move to node's local space

            // Project onto the node's axes
            float frontDist = glm::dot(pos, front);
            float upDist = glm::dot(pos, up);
            float rightDist = glm::dot(pos, right);

            if (std::abs(frontDist) <= 1.0f &&
                std::abs(upDist) <= 1.0f &&
                std::abs(rightDist) <= 1.0f)
            {
                indexes.emplace_back(meshIndex, v);
            }
        }
    }
}

//for player mainly
BoundingTree::BoundingTree(glm::vec3 position)
{
    this->max = position + glm::vec3(0.2f);
    this->max = position + glm::vec3(-0.2f);
}

BoundingTree::BoundingTree(Model &model)
{
    pair<glm::vec3, glm::vec3> box = genInitBox(model);
    min = box.first;
    max = box.second;

    glm::vec3 center = (min + max) / 2.0f;
    glm::vec3 front(max.x - center.x, 0.0f, 0.0f);
    glm::vec3 up(0.0f, max.y - center.y, 0.0f);
    glm::vec3 right(0.0f, 0.0f, max.z - center.z);

    node = new BoundingNode(center, front, up, right, 0, model);
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
        // translate the nodes
    }
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
