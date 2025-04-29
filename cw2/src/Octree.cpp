#include "Octree.h"

Octree::Octree(Model *model, glm::mat4 modelMatrix)
{
    vector<glm::vec3> positions = getPositions(model);
    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());

    for (glm::vec3 &position : positions)
    {
        this->min = glm::min(min, position);
        this->max = glm::max(max, position);
    }
    center = (min + max) / 2.0f;

    this->front = glm::length(max - center);
    this->up = glm::length(max - center);
    this->right = glm::length(max - center);

    radius = glm::length(max - min) / 2.0f;

    int size = positions.size();
    int amountPerNode = (int)size / 8;
    for (int i = 0; i < 8; i++)
    {
        node[i].parent = this;
        vector<glm::vec3> nodePositions;
        int j = positions.size() - 1;
        while (nodePositions.size() < amountPerNode && j >= 0)
        {
            nodePositions.push_back(positions[j]);
            positions.pop_back();
            j--;
        }
        if (positions.size() != 0 && i == 7)
        {
            for (glm::vec3 &position : positions)
            {
                nodePositions.push_back(position);
            }
        }
        node[i].generate(nodePositions);
    }
}

vector<glm::vec3> Octree::getPositions(Model *model)
{
    vector<glm::vec3> positions;
    for (Mesh &mesh : model->meshes)
    {
        for (Vertex vertex : mesh.vertices)
        {
            positions.push_back(vertex.Position);
        }
    }
    return positions;
}

void OctNode::generate(vector<glm::vec3> &positions)
{
    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());

    for (glm::vec3 &position : positions)
    {
        min = glm::min(min, position);
        max = glm::max(max, position);
    }

    this->center = (min + max) / 2.0f;

    this->front = glm::length(max - center);
    this->up = glm::length(max - center);
    this->right = glm::length(max - center);
}
