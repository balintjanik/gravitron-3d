#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Particle {
private:
	glm::vec4 positionMass;
	glm::vec4 velocitySize;
	glm::vec4 accelerationForce;
	glm::vec4 colorMovable;

public:
	Particle(glm::vec4 positionMass_ = glm::vec4(0.f),
		glm::vec4 velocitySize_ = glm::vec4(0.f, 0.f, 0.f, 1.f),
		glm::vec4 accelerationForce_ = glm::vec4(0.f),
		glm::vec4 colorMovable_ = glm::vec4(1.f))
		: positionMass(positionMass_),
		velocitySize(velocitySize_),
		accelerationForce(accelerationForce_),
		colorMovable(colorMovable_)
	{
	}

	Particle(glm::vec3 position_, float mass_,
		glm::vec3 velocity_, float size_,
		glm::vec3 acceleration_, float force_,
		glm::vec3 color_, bool movable_)
		: positionMass(glm::vec4(position_, mass_)),
		velocitySize(glm::vec4(velocity_, size_)),
		accelerationForce(glm::vec4(acceleration_, force_)),
		colorMovable(glm::vec4(color_, (movable_ ? 1 : 0)))
	{
	}

	static uint64_t interleaveBits(uint32_t n) {
		uint64_t x = n;
		x = (x | (x << 16)) & 0x0000FFFF0000FFFF;
		x = (x | (x << 8)) & 0x00FF00FF00FF00FF;
		x = (x | (x << 4)) & 0x0F0F0F0F0F0F0F0F;
		x = (x | (x << 2)) & 0x3333333333333333;
		x = (x | (x << 1)) & 0x5555555555555555;
		return x;
	}

	uint64_t computeMortonCode() const {
		return (interleaveBits(positionMass.x) | (interleaveBits(positionMass.y) << 1) | (interleaveBits(positionMass.z) << 2));
	}

	// Getters
	glm::vec3 getPosition() const { return glm::vec3(positionMass); }
	float getMass() const { return positionMass.w; }
	glm::vec3 getVelocity() const { return glm::vec3(velocitySize); }
	float getSize() const { return velocitySize.w; }
	glm::vec3 getAcceleration() const { return glm::vec3(accelerationForce); }
	float getForce() const { return accelerationForce.w; }
	glm::vec3 getColor() const { return glm::vec3(colorMovable); }
	bool getMovable() const { return colorMovable.w == 1.0f; }

	// Setters
	void setPosition(const glm::vec3& newPosition) { positionMass = glm::vec4(newPosition, positionMass.w); }
	void setMass(float newMass) { positionMass.w = newMass; }
	void setVelocity(const glm::vec3& newVelocity) { velocitySize = glm::vec4(newVelocity, velocitySize.w); }
	void setSize(float newSize) { velocitySize.w = newSize; }
	void setAcceleration(const glm::vec3& newAcceleration) { accelerationForce = glm::vec4(newAcceleration, accelerationForce.w); }
	void setForce(float newForce) { accelerationForce.w = newForce; }
	void setColor(const glm::vec3& newColor) { colorMovable = glm::vec4(newColor, colorMovable.w); }
	void setMovable(bool newMovable) { colorMovable.w = newMovable ? 1 : 0; }
};
