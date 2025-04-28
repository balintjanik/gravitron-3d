#include "Octree.h"

// Decide which octant an object is in based on its position
uint32_t Octant::getOctantFromPosition(glm::vec3 position)
{
    uint32_t x = uint32_t(position.x < getCenter().x);
    uint32_t y = uint32_t(position.y < getCenter().y);
    uint32_t z = uint32_t(position.z < getCenter().z);
    return  z << 2 | y << 1 | x;

}

// Create the i-th octant of a node
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

// Create all 8 octants of a node
std::vector<Octant> Octant::intoOctants(){
    std::vector<Octant> octants;
    for (uint32_t i = 0; i < 8; ++i) {
        octants.push_back(intoOctant(i));
    }
    return octants;
}

// Create an octant containing all particles in the provided list
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

// Decide if the node is a branch
bool Node::isBranch()
{
    return children != 0;
}

// Decide if the node is empty
bool Node::isEmpty()
{
    return positionMass.w == 0.0;
}

// Decide if the node is a leaf
bool Node::isLeaf()
{
    return children == 0;
}

// Clear the octree and initialize the root node with the provided octant
void Octree::clear(Octant octant)
{
    nodes.clear();
    parents.clear();
    nodes.push_back(Node(0, octant));
}

// Subdivide a node by creating its children
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

// Insert a particle into the octree
void Octree::insert(glm::vec3 position, float mass)
{
    uint32_t node = ROOT;

    // Find the leaf node where the particle is supposed to be inserted to
    while (nodes[node].isBranch())
    {
        size_t q = nodes[node].octant.getOctantFromPosition(position);
        node = nodes[node].children + q;
    }

    // If the leaf node is empty, insert the particle and return, otherwise continue
    if (nodes[node].isEmpty())
    {
        nodes[node].setPosition(position);
        nodes[node].setMass(mass);
        return;
    }

    // If the 2 particles are extremely closed, they are "merged" to avoid extreme depths in the tree, otherwise continue
    glm::vec3 nodePosition = nodes[node].getPosition();
    float nodeMass = nodes[node].getMass();

    float threshold = 1e-8;
    if (glm::all(glm::lessThan(glm::abs(position - nodePosition), glm::vec3(threshold))))
    {
        nodes[node].setMass(nodes[node].getMass() + mass);
        return;
    }

    // Subdivide until the 2 particles are in the same node
    bool bothInTheSameLeafNode = true;
    while (bothInTheSameLeafNode)
    {
        uint32_t children = subdivide(node);

        uint32_t o1 = nodes[node].octant.getOctantFromPosition(nodePosition);
        uint32_t o2 = nodes[node].octant.getOctantFromPosition(position);

        // If both fall into the same node, continue subdividing its child
        if (o1 == o2)
        {
            node = children + o1;
        }
        // Otherwise insert both particles into the correct newly created leaf node
        else
        {
            uint32_t n1 = children + o1;
            uint32_t n2 = children + o2;

            nodes[n1].setPosition(nodePosition);
            nodes[n1].setMass(nodeMass);
            nodes[n2].setPosition(position);
            nodes[n2].setMass(mass);
            bothInTheSameLeafNode = false;
        }
    }
}

// Update parent nodes' position and mass with their children's average and sum
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

// Calculate the acceleration of a particle
float Octree::calculateAcceleration(glm::vec3& r_acceleration, glm::vec3 position, float theta, float epsilon)
{
    float allForce = 0;
    r_acceleration = glm::vec3(0.f);

    float thetaSq = theta * theta;
    float epsilonSq = epsilon * epsilon;

    uint32_t node = ROOT;
    bool checkedAll = false;

    // Iterate until all particles have been accounted for
    while (!checkedAll)
    {
        Node currentNode = nodes[node];

        // Calculate distance between particle and node
        glm::vec3 distance = currentNode.getPosition() - position;
        float distanceSq = glm::dot(distance, distance);

        // Check if the node is the one the particle is in
        if (distanceSq < 1e-10)
        {
            // If so, and the next node to check is the root, then the calculations are finished
            if (currentNode.next == 0)
            {
                checkedAll = true;
            }
            // Otherwise just skip this node (to avoid zero-division during
            // force calculation due to 0 distance), and go to the next
            else
            {
                node = currentNode.next;
            }
            continue;
        }

        // If the observed node is a leaf, or it fulfills the Barnes-Hut criteria then calculate the force
        if (currentNode.isLeaf() || currentNode.octant.getSize() * currentNode.octant.getSize() < distanceSq * thetaSq)
        {
            // Calculate acceleration
            float denom = (distanceSq + epsilonSq) * sqrt(distanceSq);
            r_acceleration += distance * (currentNode.getMass() / denom);

            // Accumulate forces (only used for to color particles based on force option in the app)
            allForce += (currentNode.getMass()) / (distanceSq + epsilonSq);

            // If the next node is the root, then the calculations are finished
            if (currentNode.next == 0)
            {
                checkedAll = true;
                continue;
            }

            node = currentNode.next;
        }
        // Otherwise observe its children
        else
        {
            node = currentNode.children;
        }
    }

    return allForce;
}