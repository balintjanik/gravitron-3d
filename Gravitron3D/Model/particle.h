#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <stdexcept>

struct Particle {
private:
	glm::vec4 positionMass;
	glm::vec4 velocitySize;
	glm::vec4 accelerationForce;
	glm::vec4 colorMovable;

public:
	Particle(glm::vec4 positionMass_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4 velocitySize_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4 accelerationForce_ = glm::vec4(0.0f),
		glm::vec4 colorMovable_ = glm::vec4(1.0f))
	{
		setPosition(positionMass_);
		setMass(positionMass_.w);
		
		setVelocity(velocitySize_);
		setSize(velocitySize_.w);
		
		setAcceleration(accelerationForce_);
		setForce(accelerationForce_.w);
		
		setColor(colorMovable_);
		setMovable(colorMovable_.w);
	}

	Particle(glm::vec3 position_, float mass_,
		glm::vec3 velocity_, float size_,
		glm::vec3 acceleration_, float force_,
		glm::vec3 color_, bool movable_)
		: Particle(
			glm::vec4(position_, mass_),
			glm::vec4(velocity_, size_),
			glm::vec4(acceleration_, force_),
			glm::vec4(color_, movable_)
		)
	{
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
	void setMass(const float newMass) {
		if (newMass < 0.00001f)
			throw std::runtime_error("Incorrect value for mass: " + std::to_string(newMass) + ". Value must be larger than 0.00001");
		else
			positionMass.w = newMass;
	}
	void setVelocity(const glm::vec3& newVelocity) { velocitySize = glm::vec4(newVelocity, velocitySize.w); }
	void setSize(float newSize) {
		if (newSize < 0.1f)
			throw std::runtime_error("Incorrect value for size: " + std::to_string(newSize) + ". Value must be larger than 0.5");
		else
			velocitySize.w = newSize;
	}
	void setAcceleration(const glm::vec3& newAcceleration) { accelerationForce = glm::vec4(newAcceleration, accelerationForce.w); }
	void setForce(float newForce) {
		if (newForce < 0.0f)
			throw std::runtime_error("Incorrect value for force: " + std::to_string(newForce) + ". Value must be larger than 0");
		else
			accelerationForce.w = newForce;
	}
	void setColor(const glm::vec3& newColor) {
		if (newColor.r < 0.0f || newColor.g < 0.0f || newColor.b < 0.0f ||
			newColor.r > 1.0f || newColor.g > 1.0f || newColor.b > 1.0f)
			throw std::runtime_error("Color component values must be between 0.0 and 1.0");
		else
			colorMovable = glm::vec4(newColor, colorMovable.w);
	}
	void setMovable(bool newMovable) { colorMovable.w = newMovable ? 1 : 0; }
};
