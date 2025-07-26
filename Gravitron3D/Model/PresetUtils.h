#pragma once

#include "Particle.h"
#include "PresetType.h"
#include "PositionType.h"
#include "VelocityType.h"
#include "MassType.h"
#include "SizeType.h"

class PresetUtils {
public:
	static float randomFloat(float min, float max);

	static void calculatePositionsRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);
	static void calculatePositionsSphere(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 center, float radiusMin, float radiusMax, bool is2D = false);
	static void calculatePositionsGrid3D(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);
	static void calculatePositionsGrid2D(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);

	static void calculateVelocitiesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);
	static void calculateVelocitiesOrbit(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec4 center, float velocityScale);

	static void calculateMassesConstant(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float value);
	static void calculateMassesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float minValue, float maxValue);

	static void calculateSizesConstant(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float value);
	static void calculateSizesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float minValue, float maxValue);
};