#include "Octree.h"

Octree::Octree(Model *model, glm::mat4 modelMatrix)
{
    vector<glm::vec3> positions = getPositions(model);
    sort(positions.begin(), positions.end(),
         [](const glm::vec3 &a, const glm::vec3 &b) { return a.x < b.x; });
    glm::vec3 m(std::numeric_limits<float>::max());
    glm::vec3 a(std::numeric_limits<float>::lowest());

    for (glm::vec3 &position : positions)
    {
        m = glm::min(m, position);
        a = glm::max(a, position);
    }
    this->min = m;
    this->max = a;
    this->front = (max.x - min.x) / 2.0f;
    this->up = (max.y - min.y) / 2.0f;
    this->right = (max.z - min.z) / 2.0f;
    this->radius = glm::length(max - min) / 2.0f;
    this->center = (min + max) / 2.0f;
    cout << "radius: " << radius << endl;
    cout << "min: " << min.x << " " << min.y << " " << min.z << endl;
    cout << "max: " << max.x << " " << max.y << " " << max.z << endl;
    cout << "front: " << front << endl;
    cout << "up: " << up << endl;

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
    updatePos(modelMatrix);
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

void Octree::updatePos(glm::mat4 modelMatrix)
{
    center = glm::vec3(modelMatrix * glm::vec4(center, 1.0f));
    f = glm::vec3(modelMatrix * glm::vec4(f, 0.0f));
    u = glm::vec3(modelMatrix * glm::vec4(u, 0.0f));
    r = glm::vec3(modelMatrix * glm::vec4(r, 0.0f));

    for (int i = 0; i < 8; i++)
    {
        node[i].center =
            glm::vec3(modelMatrix * glm::vec4(node[i].center, 1.0f));
    }
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
