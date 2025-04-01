#pragma once

#include "Particle.h"
#include "PresetType.h"
#include "PositionType.h"
#include "VelocityType.h"

class PresetUtils {
public:
	static std::vector<Particle> generateParticles(int numberOfParticles, PresetType preset, PositionType position, VelocityType velocity);
	static float randomFloat(float min, float max);

	static void calculatePositionsRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);
	static void calculatePositionsSphere(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 center, float radiusMin, float radiusMax, bool is2D = false);
	static void calculatePositionsGrid3D(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);
	static void calculatePositionsGrid2D(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);

	static void calculateVelocitiesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue);
	static void calculateVelocitiesOrbit(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec4 center, float velocityScale);

private:
	static void initParticlePositions(std::vector<Particle>& r_particles, PositionType position);
	static void initParticleVelocities(std::vector<Particle>& r_particles, VelocityType velocity);

	static void initPresetGalaxy(std::vector<Particle>& r_particles);
	static void initPresetSolarSystem(std::vector<Particle>& r_particles);
	static void initPresetGalaxyCollision(std::vector<Particle>& r_particles);
	static void initPresetRandom(std::vector<Particle>& r_particles);
};