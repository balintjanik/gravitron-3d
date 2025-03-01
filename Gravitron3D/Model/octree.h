#pragma once

#include "particle.h"

struct Octant {
	glm::vec4 centerSize;

	Octant(glm::vec4 centerSize_ = glm::vec4(0)) : centerSize(centerSize_) {}

	Octant(glm::vec3 center_, float size_) : centerSize(glm::vec4(center_, size_)) { }

	uint32_t getOctantFromPosition(glm::vec3 position);

	Octant intoOctant(uint32_t i);

	std::vector<Octant> intoOctants();

	static Octant createNewContaining(std::vector<Particle>& particles);
};

struct Node {
	glm::vec4 positionMass;
	Octant octant;
	uint32_t children = 0;
	uint32_t next = 0;

	Node() {}

	Node(uint32_t next_, Octant octant_) : next(next_), octant(octant_) {}

	bool isBranch();

	bool isEmpty();

	bool isLeaf();
};

struct Octree {
	const uint32_t ROOT = 0;
	std::vector<Node> nodes;
	std::vector<uint32_t> parents;

	Octree() : nodes(), parents() {}

	void clear(Octant octant);

	uint32_t subdivide(uint32_t node);
	
	void insert(glm::vec4 positionMass);

	void propagate();

	float calculateAcceleration(glm::vec3& r_acceleration, glm::vec3 position, float theta, float epsilon);
};
