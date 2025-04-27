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
    cout << "init center: " << center.x << " " << center.y << " " << center.z
         << endl;

    this->generateNodes(index, model);
}

void BoundingNode::generateNodes(int index, Model &model)
{
    if (index == 1)
    {
        bottom = true;
        collectIndexes(model);
        return;
    }
    glm::vec3 newFront = this->front;
    glm::vec3 newUp = this->up;
    glm::vec3 newRight = this->right;
    glm::vec3 firstCenter = this->center;
    glm::vec3 secondCenter = this->center;

    if (index == 0)
    {
        newFront *= 0.5f;
        firstCenter += newFront;
        secondCenter -= newFront;
    }
    else if (index == 1)
    {
        newUp *= -0.5f;
        firstCenter += newUp;
        secondCenter -= newUp;
    }
    else if (index == 2)
    {
        newRight *= 0.5f;
        firstCenter += newRight;
        secondCenter -= newRight;
    }

    int newIndex = index + 1;

    first = new BoundingNode(firstCenter, newFront, newUp, newRight, newIndex,
                             model);
    second = new BoundingNode(secondCenter, newFront, newUp, newRight, newIndex,
                              model);
}

void BoundingNode::collectIndexes(Model &model)
{
    indexes.clear(); // If you want to clear previous data

    // Assuming center, front, up, and right are glm::vec3
    for (int meshIndex = 0; meshIndex < model.meshes.size(); ++meshIndex)
    {
        Mesh &mesh = model.meshes[meshIndex];
        for (int v = 0; v < mesh.vertices.size(); ++v)
        {
            const Vertex &vertex = mesh.vertices[v];
            const glm::vec3 &pos = vertex.Position;

            // Check if the point is within the bounding box
            if (pos.x >= (center.x - front.x) &&
                pos.x <= (center.x + front.x) && pos.y >= (center.y - up.y) &&
                pos.y <= (center.y + up.y) && pos.z >= (center.z - right.z) &&
                pos.z <= (center.z + right.z))
            {
                indexes.push_back({meshIndex, v});
            }
        }
    }
}

// for player mainly
BoundingTree::BoundingTree(glm::vec3 position)
{
    this->max = position + glm::vec3(0.2f);
    this->max = position + glm::vec3(-0.2f);
}

BoundingTree::BoundingTree(Model &model, glm::mat4 *matrix)
{
    pair<glm::vec3, glm::vec3> box = genInitBox(model);
    min = box.first;
    max = box.second;
    this->modelMatrix = matrix;
    this->model = &model;

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
