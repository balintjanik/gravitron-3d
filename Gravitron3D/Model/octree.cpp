#include "octree.h"
#include <iostream>

size_t Quad::octant(float pos_x, float pos_y, float pos_z)
{
    size_t x = size_t(pos_x < center_x);
    size_t y = size_t(pos_y < center_y);
    size_t z = size_t(pos_z < center_z);
    //return y << 1 | x;
    return  z << 2 | y << 1 | x;

}

Quad Quad::into_octant(size_t i) {
    Quad newQuad = *this;
    newQuad.size *= 0.5f;

    newQuad.center_x += (0.5f - (i & 1)) * newQuad.size;
    newQuad.center_y += (0.5f - ((i >> 1) & 1)) * newQuad.size;
    newQuad.center_z += (0.5f - (i >> 2)) * newQuad.size;
    
    return newQuad;
}

std::vector<Quad> Quad::into_octants(){
    std::vector<Quad> octants;
    for (size_t i = 0; i < 8; ++i) {
        octants.push_back(into_octant(i));
    }
    return octants;
}

Quad Quad::new_containing(std::vector<Particle>& particles)
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
        min_x = std::min(min_x, p.pos_x);
        min_y = std::min(min_y, p.pos_y);
        min_z = std::min(min_z, p.pos_z);
        max_x = std::max(max_x, p.pos_x);
        max_y = std::max(max_y, p.pos_y);
        max_z = std::max(max_z, p.pos_z);
    }

    float center_x = (min_x + max_x) * 0.5;
    float center_y = (min_y + max_y) * 0.5;
    float center_z = (min_z + max_z) * 0.5;
    float size = std::max(std::max((max_x - min_x), (max_y - min_y)), (max_z - min_z));
    
    return Quad(center_x, center_y, center_z, size);
}

bool Node::is_branch()
{
    return children != 0;
}

bool Node::is_empty()
{
    return mass == 0.0;
}

bool Node::is_leaf()
{
    return children == 0;
}

void Octree::clear(Quad quad)
{
    nodes.clear();
    parents.clear();
    nodes.push_back(Node(0, quad));
}

size_t Octree::subdivide(size_t node) {
    parents.push_back(node);

    size_t children = nodes.size();
    nodes[node].children = children;

    size_t nexts[8] = {
        children + 1,
        children + 2,
        children + 3,
        children + 4,
        children + 5,
        children + 6,
        children + 7,
        nodes[node].next
    };

    auto quads = nodes[node].quad.into_octants();

    for (size_t i = 0; i < 8; ++i) {
        nodes.push_back(Node(nexts[i], quads[i]));
    }

    return children;
}

void Octree::insert(float pos_x, float pos_y, float pos_z, float mass)
{
    size_t node = ROOT;

    while (nodes[node].is_branch())
    {
        size_t q = nodes[node].quad.octant(pos_x, pos_y, pos_z);
        node = nodes[node].children + q;
    }

    if (nodes[node].is_empty())
    {
        nodes[node].pos_x = pos_x;
        nodes[node].pos_y = pos_y;
        nodes[node].pos_z = pos_z;
        nodes[node].mass = mass;
        return;
    }

    float px = nodes[node].pos_x;
    float py = nodes[node].pos_y;
    float pz = nodes[node].pos_z;
    float m = nodes[node].mass;

    float threshold = 1e-8;
    if (fabs(pos_x - px) < threshold && fabs(pos_y - py) < threshold && fabs(pos_z - pz) < threshold)
    {
        nodes[node].mass += mass;
        return;
    }

    bool both_in_same_leaf_node = true;
    while (both_in_same_leaf_node)
    {
        size_t children = subdivide(node);

        size_t q1 = nodes[node].quad.octant(px, py, pz);
        size_t q2 = nodes[node].quad.octant(pos_x, pos_y, pos_z);

        if (q1 == q2)
        {
            node = children + q1;
        }
        else
        {
            size_t n1 = children + q1;
            size_t n2 = children + q2;

            nodes[n1].pos_x = px;
            nodes[n1].pos_y = py;
            nodes[n1].pos_z = pz;
            nodes[n1].mass = m;
            nodes[n2].pos_x = pos_x;
            nodes[n2].pos_y = pos_y;
            nodes[n2].pos_z = pos_z;
            nodes[n2].mass = mass;
            both_in_same_leaf_node = false;
        }
    }
}

void Octree::propagate()
{
    std::reverse(parents.begin(), parents.end());

    for (size_t node : parents)
    {
        size_t i = nodes[node].children;

        nodes[node].pos_x = nodes[i].pos_x * nodes[i].mass
            + nodes[i + 1].pos_x * nodes[i + 1].mass
            + nodes[i + 2].pos_x * nodes[i + 2].mass
            + nodes[i + 3].pos_x * nodes[i + 3].mass
            + nodes[i + 4].pos_x * nodes[i + 4].mass
            + nodes[i + 5].pos_x * nodes[i + 5].mass
            + nodes[i + 6].pos_x * nodes[i + 6].mass
            + nodes[i + 7].pos_x * nodes[i + 7].mass;

        nodes[node].pos_y = nodes[i].pos_y * nodes[i].mass
            + nodes[i + 1].pos_y * nodes[i + 1].mass
            + nodes[i + 2].pos_y * nodes[i + 2].mass
            + nodes[i + 3].pos_y * nodes[i + 3].mass
            + nodes[i + 4].pos_y * nodes[i + 4].mass
            + nodes[i + 5].pos_y * nodes[i + 5].mass
            + nodes[i + 6].pos_y * nodes[i + 6].mass
            + nodes[i + 7].pos_y * nodes[i + 7].mass;

        nodes[node].pos_z = nodes[i].pos_z * nodes[i].mass
            + nodes[i + 1].pos_z * nodes[i + 1].mass
            + nodes[i + 2].pos_z * nodes[i + 2].mass
            + nodes[i + 3].pos_z * nodes[i + 3].mass
            + nodes[i + 4].pos_z * nodes[i + 4].mass
            + nodes[i + 5].pos_z * nodes[i + 5].mass
            + nodes[i + 6].pos_z * nodes[i + 6].mass
            + nodes[i + 7].pos_z * nodes[i + 7].mass;
        
        nodes[node].mass = nodes[i].mass
            + nodes[i + 1].mass
            + nodes[i + 2].mass
            + nodes[i + 3].mass
            + nodes[i + 4].mass
            + nodes[i + 5].mass
            + nodes[i + 6].mass
            + nodes[i + 7].mass;

        float mass = nodes[node].mass;
        nodes[node].pos_x /= mass;
        nodes[node].pos_y /= mass;
        nodes[node].pos_z /= mass;
    }
}

float Octree::acc(float& acc_x, float& acc_y, float& acc_z, float pos_x, float pos_y, float pos_z,  float theta, float epsilon)
{
    float all_force = 0;
    acc_x = 0.0;
    acc_y = 0.0;
    acc_z = 0.0;

    float theta_sq = theta * theta;
    float epsilon_sq = epsilon * epsilon;

    size_t node = ROOT;
    bool checked_all = false;

    while (!checked_all)
    {
        Node n = nodes[node];

        float dx = n.pos_x - pos_x;
        float dy = n.pos_y - pos_y;
        float dz = n.pos_z - pos_z;
        float dsq = dx * dx + dy * dy + dz * dz;

        // FONTOS - ez alapból nem volt benne, de, ha ez nincs,
        // akkor a saját node-jánál is számolni akar,
        // zéróosztó lesz és beakad meghal a program
        // Note: floating-point division miatt nem == 0.0 hanem ez lett
        if (dsq < 1e-10)
        {
            if (n.next == 0)
            {
                checked_all = true;
            }
            else
            {
                node = n.next;
            }
            continue;
        }

        if (n.is_leaf() || n.quad.size * n.quad.size < dsq * theta_sq)
        {
            float denom = (dsq + epsilon_sq) * sqrt(dsq);
            acc_x += dx * (n.mass / denom);
            acc_y += dy * (n.mass / denom);
            acc_z += dz * (n.mass / denom);

            all_force += (n.mass) / (dsq + epsilon_sq);

            if (n.next == 0)
            {
                checked_all = true;
                continue;
            }

            node = n.next;
        }
        else
        {
            node = n.children;
        }
    }

    return all_force;
}