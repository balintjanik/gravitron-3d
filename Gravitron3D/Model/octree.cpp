#include "Octree.h"

uint32_t Octant::getOctantFromPosition(glm::vec3 position)
{
    uint32_t x = uint32_t(position.x < getCenter().x);
    uint32_t y = uint32_t(position.y < getCenter().y);
    uint32_t z = uint32_t(position.z < getCenter().z);
    return  z << 2 | y << 1 | x;

}

Octant Octant::intoOctant(uint32_t i) {
    Octant newOctant = *this;
    float size = newOctant.getSize() * 0.5f;

    float center_x = newOctant.getCenter().x + (0.5f - (i & 1)) * newOctant.getSize();
    float center_y = newOctant.getCenter().y + (0.5f - ((i >> 1) & 1)) * newOctant.getSize();
    float center_z = newOctant.getCenter().z + (0.5f - (i >> 2)) * newOctant.getSize();
    
    newOctant.setCenter(glm::vec3(center_x, center_y, center_z));
    newOctant.setSize(size);

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
    if (particles.size() == 0)
        return Octant(glm::vec4(0.0f));

    float min_x = FLT_MAX;
    float min_y = FLT_MAX;
    float min_z = FLT_MAX;
    float max_x = -FLT_MAX;
    float max_y = -FLT_MAX;
    float max_z = -FLT_MAX;
    for (int i = 0; i < particles.size(); i++)
    {
        Particle p = particles[i];
        min_x = std::min(min_x, p.getPosition().x);
        min_y = std::min(min_y, p.getPosition().y);
        min_z = std::min(min_z, p.getPosition().z);
        max_x = std::max(max_x, p.getPosition().x);
        max_y = std::max(max_y, p.getPosition().y);
        max_z = std::max(max_z, p.getPosition().z);
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

void Octree::insert(glm::vec3 position, float mass)
{
    uint32_t node = ROOT;

    while (nodes[node].isBranch())
    {
        size_t q = nodes[node].octant.getOctantFromPosition(position);
        node = nodes[node].children + q;
    }

    if (nodes[node].isEmpty())
    {
        nodes[node].setPosition(position);
        nodes[node].setMass(mass);
        return;
    }

    glm::vec3 nodePosition = nodes[node].getPosition();
    float nodeMass = nodes[node].getMass();

    float threshold = 1e-8;
    if (glm::all(glm::lessThan(glm::abs(position - nodePosition), glm::vec3(threshold))))
    {
        nodes[node].setMass(nodes[node].getMass() + mass);
        return;
    }

    bool bothInTheSameLeafNode = true;
    while (bothInTheSameLeafNode)
    {
        size_t children = subdivide(node);

        size_t o1 = nodes[node].octant.getOctantFromPosition(nodePosition);
        size_t o2 = nodes[node].octant.getOctantFromPosition(position);

        if (o1 == o2)
        {
            node = children + o1;
        }
        else
        {
            size_t n1 = children + o1;
            size_t n2 = children + o2;

            nodes[n1].setPosition(nodePosition);
            nodes[n1].setMass(nodeMass);
            nodes[n2].setPosition(position);
            nodes[n2].setMass(mass);
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
            nodes[i].getPosition() * nodes[i].getMass() +
            nodes[i + 1].getPosition() * nodes[i + 1].getMass() +
            nodes[i + 2].getPosition() * nodes[i + 2].getMass() +
            nodes[i + 3].getPosition() * nodes[i + 3].getMass() +
            nodes[i + 4].getPosition() * nodes[i + 4].getMass() +
            nodes[i + 5].getPosition() * nodes[i + 5].getMass() +
            nodes[i + 6].getPosition() * nodes[i + 6].getMass() +
            nodes[i + 7].getPosition() * nodes[i + 7].getMass();

        float totalMass =
            nodes[i].getMass() +
            nodes[i + 1].getMass() +
            nodes[i + 2].getMass() +
            nodes[i + 3].getMass() +
            nodes[i + 4].getMass() +
            nodes[i + 5].getMass() +
            nodes[i + 6].getMass() +
            nodes[i + 7].getMass();
        
        weightedPosition /= totalMass;

        nodes[node].setPosition(weightedPosition);
        nodes[node].setMass(totalMass);
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

        glm::vec3 distance = currentNode.getPosition() - position;
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

        if (currentNode.isLeaf() || currentNode.octant.getSize() * currentNode.octant.getSize() < distanceSq * thetaSq)
        {
            float denom = (distanceSq + epsilonSq) * sqrt(distanceSq);
            r_acceleration += distance * (currentNode.getMass() / denom);

            allForce += (currentNode.getMass()) / (distanceSq + epsilonSq);

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