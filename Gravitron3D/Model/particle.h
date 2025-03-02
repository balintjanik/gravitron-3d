#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Particle {
private:
	glm::vec4 positionMass;
	glm::vec4 velocitySize;
	glm::vec4 accelerationForce;

public:
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

	// Getters
	glm::vec3 getPosition() const { return glm::vec3(positionMass); }
	float getMass() const { return positionMass.w; }
	glm::vec3 getVelocity() const { return glm::vec3(velocitySize); }
	float getSize() const { return velocitySize.w; }
	glm::vec3 getAcceleration() const { return glm::vec3(accelerationForce); }
	float getForce() const { return accelerationForce.w; }

	// Setters
	void setPosition(const glm::vec3& newPosition) { positionMass = glm::vec4(newPosition, positionMass.w); }
	void setMass(float newMass) { positionMass.w = newMass; }
	void setVelocity(const glm::vec3& newVelocity) { velocitySize = glm::vec4(newVelocity, velocitySize.w); }
	void setSize(float newSize) { velocitySize.w = newSize; }
	void setAcceleration(const glm::vec3& newAcceleration) { accelerationForce = glm::vec4(newAcceleration, accelerationForce.w); }
	void setForce(float newForce) { accelerationForce.w = newForce; }
};
