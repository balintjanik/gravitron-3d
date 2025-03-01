#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Particle {
	glm::vec4 positionMass;
	glm::vec4 velocitySize;
	glm::vec4 accelerationForce;

	Particle(glm::vec4 positionMass_ = glm::vec4(0.f),
		glm::vec4 velocitySize_ = glm::vec4(0.f),
		glm::vec4 accelerationForce_ = glm::vec4(0.f))
		: positionMass(positionMass_),
		velocitySize(velocitySize_),
		accelerationForce(accelerationForce_)
	{
	}

	Particle(glm::vec3 position_, float mass_,
		glm::vec3 velocity_, float size_,
		glm::vec3 acceleration_, float force_)
		: positionMass(glm::vec4(position_, mass_)),
		velocitySize(glm::vec4(velocity_, size_)),
		accelerationForce(glm::vec4(acceleration_, force_))
	{
	}
};
