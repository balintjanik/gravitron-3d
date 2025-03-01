#include "octree.h"

uint32_t Octant::getOctantFromPosition(glm::vec3 position)
{
    uint32_t x = uint32_t(position.x < centerSize.x);
    uint32_t y = uint32_t(position.y < centerSize.y);
    uint32_t z = uint32_t(position.z < centerSize.z);
    return  z << 2 | y << 1 | x;

}

Octant Octant::intoOctant(uint32_t i) {
    Octant newOctant = *this;
    float size = newOctant.centerSize.w * 0.5f;

    float center_x = newOctant.centerSize.x + (0.5f - (i & 1)) * newOctant.centerSize.w;
    float center_y = newOctant.centerSize.y + (0.5f - ((i >> 1) & 1)) * newOctant.centerSize.w;
    float center_z = newOctant.centerSize.z + (0.5f - (i >> 2)) * newOctant.centerSize.w;
    
    newOctant.centerSize = glm::vec4(center_x, center_y, center_z, size);

    return newOctant;
}

std::vector<Octant> Octant::intoOctants(){
    std::vector<Octant> octants;
    for (uint32_t i = 0; i < 8; ++i) {
        octants.push_back(intoOctant(i));
    }
    return octants;
}

Octant Octant::createNewContaining(std::vector<Particle>& particles)
{
    float min_x = FLT_MAX;
    float min_y = FLT_MAX;
    float min_z = FLT_MAX;
    float max_x = -FLT_MAX;
    float max_y = -FLT_MAX;
    float max_z = -FLT_MAX;
    for (int i = 0; i < particles.size(); i++)
    {
        Particle p = particles[i];
        min_x = std::min(min_x, p.positionMass.x);
        min_y = std::min(min_y, p.positionMass.y);
        min_z = std::min(min_z, p.positionMass.z);
        max_x = std::max(max_x, p.positionMass.x);
        max_y = std::max(max_y, p.positionMass.y);
        max_z = std::max(max_z, p.positionMass.z);
    }

    float center_x = (min_x + max_x) * 0.5;
    float center_y = (min_y + max_y) * 0.5;
    float center_z = (min_z + max_z) * 0.5;
    float size = std::max(std::max((max_x - min_x), (max_y - min_y)), (max_z - min_z));
    
    return Octant(glm::vec4(center_x, center_y, center_z, size));
}

bool Node::isBranch()
{
    return children != 0;
}

bool Node::isEmpty()
{
    return positionMass.w == 0.0;
}

bool Node::isLeaf()
{
    return children == 0;
}

void Octree::clear(Octant octant)
{
    nodes.clear();
    parents.clear();
    nodes.push_back(Node(0, octant));
}

uint32_t Octree::subdivide(uint32_t node) {
    parents.push_back(node);

    uint32_t children = nodes.size();
    nodes[node].children = children;

    uint32_t nexts[8] = {
        children + 1,
        children + 2,
        children + 3,
        children + 4,
        children + 5,
        children + 6,
        children + 7,
        nodes[node].next
    };

    auto octants = nodes[node].octant.intoOctants();

    for (uint32_t i = 0; i < 8; ++i) {
        nodes.push_back(Node(nexts[i], octants[i]));
    }

    return children;
}

void Octree::insert(glm::vec4 positionMass)
{
    uint32_t node = ROOT;

    while (nodes[node].isBranch())
    {
        size_t q = nodes[node].octant.getOctantFromPosition(glm::vec3(positionMass));
        node = nodes[node].children + q;
    }

    if (nodes[node].isEmpty())
    {
        nodes[node].positionMass = positionMass;
        return;
    }

    glm::vec4 nodePositionMass = nodes[node].positionMass;

    float threshold = 1e-8;
    if (glm::all(glm::lessThan(glm::abs(glm::vec3(positionMass) - glm::vec3(nodePositionMass)), glm::vec3(threshold))))
    {
        nodes[node].positionMass.w += positionMass.w;
        return;
    }

    bool bothInTheSameLeafNode = true;
    while (bothInTheSameLeafNode)
    {
        size_t children = subdivide(node);

        size_t o1 = nodes[node].octant.getOctantFromPosition(glm::vec3(nodePositionMass));
        size_t o2 = nodes[node].octant.getOctantFromPosition(glm::vec3(positionMass));

        if (o1 == o2)
        {
            node = children + o1;
        }
        else
        {
            size_t n1 = children + o1;
            size_t n2 = children + o2;

            nodes[n1].positionMass = nodePositionMass;
            nodes[n2].positionMass = positionMass;
            bothInTheSameLeafNode = false;
        }
    }
}

void Octree::propagate()
{
    std::reverse(parents.begin(), parents.end());

    for (size_t node : parents)
    {
        size_t i = nodes[node].children;

        glm::vec3 weightedPosition =
            glm::vec3(nodes[i].positionMass) * nodes[i].positionMass.w +
            glm::vec3(nodes[i + 1].positionMass) * nodes[i + 1].positionMass.w +
            glm::vec3(nodes[i + 2].positionMass) * nodes[i + 2].positionMass.w +
            glm::vec3(nodes[i + 3].positionMass) * nodes[i + 3].positionMass.w +
            glm::vec3(nodes[i + 4].positionMass) * nodes[i + 4].positionMass.w +
            glm::vec3(nodes[i + 5].positionMass) * nodes[i + 5].positionMass.w +
            glm::vec3(nodes[i + 6].positionMass) * nodes[i + 6].positionMass.w +
            glm::vec3(nodes[i + 7].positionMass) * nodes[i + 7].positionMass.w;

        float totalMass =
            nodes[i].positionMass.w +
            nodes[i + 1].positionMass.w +
            nodes[i + 2].positionMass.w +
            nodes[i + 3].positionMass.w +
            nodes[i + 4].positionMass.w +
            nodes[i + 5].positionMass.w +
            nodes[i + 6].positionMass.w +
            nodes[i + 7].positionMass.w;
        
        weightedPosition /= totalMass;

        nodes[node].positionMass = glm::vec4(weightedPosition, totalMass);
    }
}

float Octree::calculateAcceleration(glm::vec3& r_acceleration, glm::vec3 position, float theta, float epsilon)
{
    float allForce = 0;
    r_acceleration = glm::vec3(0.f);

    float thetaSq = theta * theta;
    float epsilonSq = epsilon * epsilon;

    uint32_t node = ROOT;
    bool checkedAll = false;

    while (!checkedAll)
    {
        Node currentNode = nodes[node];

        glm::vec3 distance = glm::vec3(currentNode.positionMass) - position;
        float distanceSq = glm::dot(distance, distance);

        // FONTOS - ez alapból nem volt benne, de, ha ez nincs,
        // akkor a saját node-jánál is számolni akar,
        // zéróosztó lesz és beakad meghal a program
        // Note: floating-point division miatt nem == 0.0 hanem ez lett
        if (distanceSq < 1e-10)
        {
            if (currentNode.next == 0)
            {
                checkedAll = true;
            }
            else
            {
                node = currentNode.next;
            }
            continue;
        }

        if (currentNode.isLeaf() || currentNode.octant.centerSize.w * currentNode.octant.centerSize.w < distanceSq * thetaSq)
        {
            float denom = (distanceSq + epsilonSq) * sqrt(distanceSq);
            r_acceleration += distance * (currentNode.positionMass.w / denom);

            allForce += (currentNode.positionMass.w) / (distanceSq + epsilonSq);

            if (currentNode.next == 0)
            {
                checkedAll = true;
                continue;
            }

            node = currentNode.next;
        }
        else
        {
            node = currentNode.children;
        }
    }

    return allForce;
}