#include "PresetUtils.h"

float PresetUtils::randomFloat(float min, float max)
{
	// TODO: this assumes max>min
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random * range) + min;
}

std::vector<Particle> PresetUtils::generateParticles(int numberOfParticles, PresetType preset, PositionType position, VelocityType velocity) {
	std::vector<Particle> particles;

	for (int i = 0; i < numberOfParticles; i++) {
		particles.push_back(Particle());
		particles[i].setMass(1.f); // TODO: mass setting?
	}

	initParticlePositions(particles, position);

	initParticleVelocities(particles, velocity);

	return particles;
}

void PresetUtils::initParticlePositions(std::vector<Particle>& r_particles, PositionType position) {
	switch (position)
	{
	case POSITION_RANDOM: // TODO: specify range?
		glm::vec3 min = glm::vec3(-50.f, -50.f, -50.f);
		glm::vec3 max = glm::vec3( 50.f,  50.f,  50.f);
		for (int i = 0; i < r_particles.size(); i++) {

			r_particles[i].setPosition(glm::vec3(randomFloat(min.x, max.x), randomFloat(min.y, max.y), randomFloat(min.z, max.z)));
		}
		break;
	case POSITION_SPHERE: // TODO: specify radius?
		break;
	case POSITION_DISK: // TODO: specify radius?
		break;
	case POSITION_GRID_3D:
		break;
	case POSITION_GRID_2D:
		break;
	default:
		throw "Invalid position type.";
		break;
	}
}

void PresetUtils::initParticleVelocities(std::vector<Particle>& r_particles, VelocityType velocity) {
	switch (velocity)
	{
	case VELOCITY_RANDOM: // TODO: specify range?
		break;
	case VELOCITY_ZERO:
		break;
	case VELOCITY_ORBIT:
		break;
	case VELOCITY_TOWARD_CENTER:
		break;
	default:
		throw "Invalid velocity type.";
		break;
	}
}