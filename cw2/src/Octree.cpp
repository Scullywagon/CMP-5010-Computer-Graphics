#include "Octree.h"

Octree::Octree(Model *model, glm::mat4 modelMatrix)
{
    vector<pair<int, int>> triangles = getTriangles(model);
    glm::vec3 m(std::numeric_limits<float>::max());
    glm::vec3 a(std::numeric_limits<float>::lowest());

    for (Mesh &mesh : model->meshes)
    {
        for (Vertex &v : mesh.vertices)
        {
            m = glm::min(m, v.Position);
            a = glm::max(a, v.Position);
        }
    }
    this->min = m;
    this->max = a;
    this->front = (max.x - min.x) / 2.0f;
    this->up = (max.y - min.y) / 2.0f;
    this->right = (max.z - min.z) / 2.0f;
    this->radius = glm::length(max - min) / 2.0f;
    this->center = (min + max) / 2.0f;

    splitTriangles(triangles, model);
    updatePos(modelMatrix);
}

vector<pair<int, int>> Octree::getTriangles(Model *model)
{
    vector<pair<int, int>> tirangles;
    for (int x = 0; x < model->meshes.size(); x++)
    {
        Mesh &mesh = model->meshes[x];
        for (int i = 0; i < mesh.indices.size(); i += 3)
        {
            tirangles.push_back(std::make_pair(x, i));
        }
    }
    return tirangles;
}

void Octree::updatePos(glm::mat4 modelMatrix)
{
    center = glm::vec3(modelMatrix * glm::vec4(center, 1.0f));
    f = glm::vec3(modelMatrix * glm::vec4(f, 0.0f));
    u = glm::vec3(modelMatrix * glm::vec4(u, 0.0f));
    r = glm::vec3(modelMatrix * glm::vec4(r, 0.0f));

    for (int i = 0; i < node.size(); i++)
    {
        node[i].center =
            glm::vec3(modelMatrix * glm::vec4(node[i].center, 1.0f));
    }
}

void Octree::translate(glm::vec3 translation)
{
    center += translation;

    for (int i = 0; i < node.size(); i++)
    {
        node[i].center += translation;
    }
}

void Octree::rotate(float rotation, glm::vec3 axis)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, axis);
    f = glm::vec3(rotationMatrix * glm::vec4(f, 0.0f));
    u = glm::vec3(rotationMatrix * glm::vec4(u, 0.0f));
    r = glm::vec3(rotationMatrix * glm::vec4(r, 0.0f));

    for (int i = 0; i < node.size(); i++)
    {
        node[i].center =
            glm::vec3(rotationMatrix * glm::vec4(node[i].center, 1.0f));
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

void Octree::splitTriangles(vector<pair<int, int>> &triangles, Model *model)
{
    struct IntermediaryNode
    {
        vector<pair<int, int>> triangles;
        glm::vec3 center;
        glm::vec3 min, max;
    };

    IntermediaryNode rootNode;
    rootNode.triangles = triangles;
    rootNode.min = min;
    rootNode.max = max;
    rootNode.center = (min + max) / 2.0f;

    auto getLongestAxis = [](const IntermediaryNode &node) -> int
    {
        glm::vec3 dimensions = node.max - node.min;
        if (dimensions.x >= dimensions.y && dimensions.x >= dimensions.z)
            return 0; // X-axis
        else if (dimensions.y >= dimensions.x && dimensions.y >= dimensions.z)
            return 1; // Y-axis
        else
            return 2; // Z-axis
    };

    auto splitNode = [&](const IntermediaryNode &node)
        -> pair<IntermediaryNode, IntermediaryNode>
    {
        IntermediaryNode left, right;
        int axis = getLongestAxis(node);

        float splitValue = (node.min[axis] + node.max[axis]) / 2.0f;

        left.min = node.min;
        left.max = node.max;
        left.max[axis] = splitValue;

        right.min = node.min;
        right.max = node.max;
        right.min[axis] = splitValue;

        left.center = (left.min + left.max) / 2.0f;
        right.center = (right.min + right.max) / 2.0f;

        for (const auto &tri : node.triangles)
        {
            glm::vec3 p0 =
                model->meshes[tri.first]
                    .vertices[model->meshes[tri.first].indices[tri.second]]
                    .Position;
            glm::vec3 p1 =
                model->meshes[tri.first]
                    .vertices[model->meshes[tri.first].indices[tri.second + 1]]
                    .Position;
            glm::vec3 p2 =
                model->meshes[tri.first]
                    .vertices[model->meshes[tri.first].indices[tri.second + 2]]
                    .Position;

            glm::vec3 centroid = (p0 + p1 + p2) / 3.0f;

            if (centroid[axis] < splitValue)
            {
                left.triangles.push_back(tri);
            }
            else
            {
                right.triangles.push_back(tri);
            }
        }

        return {left, right};
    };

    std::vector<IntermediaryNode> nodes = {rootNode};

    for (int level = 0; level < 5; ++level)
    {
        std::vector<IntermediaryNode> nextLevel;
        for (auto &node : nodes)
        {
            auto [left, right] = splitNode(node);
            nextLevel.push_back(left);
            nextLevel.push_back(right);
        }
        nodes = std::move(nextLevel);
    }

    IntermediaryNode finalNodes[32];
    std::copy(nodes.begin(), nodes.end(), finalNodes);

    for (int i = 0; i < 32; i++)
    {
        OctNode n;
        if (finalNodes[i].triangles.empty())
            continue;
        n.parent = this;
        n.center = finalNodes[i].center;
        n.front = (finalNodes[i].max.x - finalNodes[i].min.x) / 2.0f;
        n.up = (finalNodes[i].max.y - finalNodes[i].min.y) / 2.0f;
        n.right = (finalNodes[i].max.z - finalNodes[i].min.z) / 2.0f;

        node.push_back(n);

        cout << "n center:" << n.center.x << " " << n.center.y << " "
             << n.center.z << endl;
        cout << "front:" << n.front << " up:" << n.up << " right:" << n.right
             << endl;
    }
    cout << "node size: " << node.size() << endl;

    triangles.clear();
}
