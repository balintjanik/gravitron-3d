#pragma once

#include "Particle.h"
#include "PresetType.h"
#include "PositionType.h"
#include "VelocityType.h"

class PresetUtils {
public:
	static std::vector<Particle> generateParticles(int numberOfParticles, PresetType preset, PositionType position, VelocityType velocity);
	static float randomFloat(float min, float max);

private:
	static void initParticlePositions(std::vector<Particle>& r_particles, PositionType position);
	static void initParticleVelocities(std::vector<Particle>& r_particles, VelocityType velocity);
};