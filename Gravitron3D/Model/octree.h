#pragma once
#include <vector>
#include "particle.h"

struct Quad {
	float center_x, center_y, center_z;
	float size;

	Quad(float center_x = 0, float center_y = 0, float center_z = 0, float size = 0)
		: center_x(center_x), center_y(center_y), center_z(center_z), size(size)
	{
	}

	size_t octant(float pos_x, float pos_y, float pos_z);
	Quad into_octant(size_t i);
	std::vector<Quad> into_octants();

	static Quad new_containing(std::vector<Particle>& particles);
};

struct Node {
	size_t children = 0;
	size_t next = 0;
	Quad quad;
	float pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;
	float mass = 0.0;

	Node() {}
	Node(size_t next, Quad quad) : next(next), quad(quad) {}

	bool is_branch();
	bool is_empty();
	bool is_leaf();
};

struct Octree {
	const std::size_t ROOT = 0;
	std::vector<Node> nodes;
	std::vector<size_t> parents;

	Octree() : nodes(), parents() {}

	void clear(Quad quad);
	size_t subdivide(size_t node);
	
	void insert(float pos_x, float pos_y, float pos_z, float mass);

	void propagate();

	float acc(float& acc_x, float& acc_y, float& acc_z, float pos_x, float pos_y, float pos_z, float theta, float epsilon);
};
