#pragma once

#include "Particle.h"

struct Octant {

private:
	glm::vec4 centerSize;


public:
	Octant(glm::vec4 centerSize_ = glm::vec4(0)) : centerSize(centerSize_) {}
	Octant(glm::vec3 center_, float size_) : centerSize(glm::vec4(center_, size_)) { }

	uint32_t getOctantFromPosition(glm::vec3 position);
	Octant intoOctant(uint32_t i);
	std::vector<Octant> intoOctants();
	static Octant createNewContaining(std::vector<Particle>& particles);

	// Getters
	glm::vec3 getCenter() const { return glm::vec3(centerSize); }
	float getSize() const { return centerSize.w; }

	// Setters
	void setCenter(const glm::vec3& newCenter) { centerSize = glm::vec4(newCenter, centerSize.w); }
	void setSize(float newSize) { centerSize.w = newSize; }
};

struct Node {
private:
	glm::vec4 positionMass = glm::vec4(0.f);

public:
	Octant octant;
	uint32_t children = 0;
	uint32_t next = 0;

	Node() {}
	Node(uint32_t next_, Octant octant_) : next(next_), octant(octant_) {}

	bool isBranch();
	bool isEmpty();
	bool isLeaf();

	// Getters
	glm::vec3 getPosition() const { return glm::vec3(positionMass); }
	float getMass() const { return positionMass.w; }

	// Setters
	void setPosition(const glm::vec3& newPosition) { positionMass = glm::vec4(newPosition, positionMass.w); }
	void setMass(float newMass) { positionMass.w = newMass; }
};

struct Octree {
	const uint32_t ROOT = 0;
	std::vector<Node> nodes;
	std::vector<uint32_t> parents;

	Octree() : nodes(), parents() {}

	void clear(Octant octant);
	uint32_t subdivide(uint32_t node);
	void insert(glm::vec3 position, float mass);
	void propagate();
	float calculateAcceleration(glm::vec3& r_acceleration, glm::vec3 position, float theta, float epsilon);
};
