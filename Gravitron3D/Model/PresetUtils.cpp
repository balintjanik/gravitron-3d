#include "PresetUtils.h"

std::vector<Particle> PresetUtils::generateParticles(int numberOfParticles, PresetType preset, PositionType position, VelocityType velocity) {
	std::vector<Particle> particles;

	for (int i = 0; i < numberOfParticles; i++) {
		particles.push_back(Particle());
		particles[i].setMass(1.f); // TODO: mass setting?
	}

	initParticlePositions(particles, position);

	initParticleVelocities(particles, velocity);
}

void initParticlePositions(std::vector<Particle>& r_particles, PositionType position) {
	switch (position)
	{
	case RANDOM: // TODO: specify range?
		glm::vec3 min = glm::vec3(-200.f, -200.f, -200.f);
		glm::vec3 max = glm::vec3( 200.f,  200.f,  200.f);
		for (int i = 0; i < r_particles.size(); i++) {

			r_particles[i].setPosition(glm::vec3(randomFloat(min.x, max.x), randomFloat(min.y, max.y), randomFloat(min.z, max.z)));
		}
		break;
	case SPHERE: // TODO: specify radius?
		break;
	case DISK: // TODO: specify radius?
		break;
	case GRID_3D:
		break;
	case GRID_2D:
		break;
	default:
		throw "Invalid position type.";
		break;
	}
}

void initParticleVelocities(std::vector<Particle>& r_particles, VelocityType velocity) {
	switch (velocity)
	{
	case RANDOM: // TODO: specify range?
		break;
	case ZERO:
		break;
	case ORBIT:
		break;
	case TOWARD_CENTER:
		break;
	default:
		throw "Invalid velocity type.";
		break;
	}
}

float randomFloat(float min, float max)
{
	// TODO: this assumes max>min
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random * range) + min;
}