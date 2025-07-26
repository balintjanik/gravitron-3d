#include "PresetUtils.h"
#include <stdexcept>

// Generate random float between minimum and maximum values (both inclusive)
float PresetUtils::randomFloat(float min, float max)
{
	if (max < min)
		throw std::runtime_error("Minimum value must not be larger than maximum value");

	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random * range) + min;
}

// Initialize the particles' positions randomly between minValue
// and maxValue from rangeMin to rangeMax in the list
void PresetUtils::calculatePositionsRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setPosition(glm::vec3(randomFloat(minValue.x, maxValue.x), randomFloat(minValue.y, maxValue.y), randomFloat(minValue.z, maxValue.z)));
	}
}

// Initialize the particles' positions in a sphere between radiusMin and
// radiusMax at the specified center from rangeMin to rangeMax in the list
void PresetUtils::calculatePositionsSphere(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 center, float radiusMin, float radiusMax, bool is2D) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	if (radiusMin < 0)
		throw std::runtime_error("Minimum radius must be a non-negative number");

	if (radiusMax < radiusMin)
		throw std::runtime_error("Maximum radius must not be smaller than minimum radius");

	for (int i = rangeMin; i < rangeMax; i++)
	{
		// Generate a random position in a sphere
		float radius = randomFloat(radiusMin, radiusMax);
		float theta = randomFloat(0.0f, 2 * 3.1415f);
		float phi = randomFloat(0.0f, 3.1415f);

		// Convert polar to Cartesian coordinates
		float px = center.x + radius * sin(phi) * cos(theta);
		float py = center.y + radius * sin(phi) * sin(theta);
		float pz = center.z + radius * cos(phi);

		// Only use x and z axes for 2D (disk)
		if (is2D) {
			px = center.x + radius * cos(theta);
			py = center.y;
			pz = center.z + radius * sin(theta);
		}

		r_particles[i].setPosition(glm::vec3(px, py, pz));
	}
}

// Initialize the particles' positions evenly between minValue
// and maxValue from rangeMin to rangeMax in the list
void PresetUtils::calculatePositionsGrid3D(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	int totalParticles = rangeMax - rangeMin;
	if (totalParticles <= 0) return;

	int gridSize = std::ceil(std::cbrt(totalParticles));
	int count = 0;

	glm::vec3 stepSize = (maxValue - minValue) / glm::vec3(gridSize - 1, gridSize - 1, gridSize - 1);

	for (int x = 0; x < gridSize && count < totalParticles; x++) {
		for (int y = 0; y < gridSize && count < totalParticles; y++) {
			for (int z = 0; z < gridSize && count < totalParticles; z++) {
				int index = rangeMin + count;
				glm::vec3 position = minValue + glm::vec3(x, y, z) * stepSize;
				r_particles[index].setPosition(position);
				count++;

				if (count >= totalParticles)
					return;
			}
		}
	}
}

// Initialize the particles' positions evenly on X and Z axes between
// minValue and maxValue from rangeMin to rangeMax in the list
void PresetUtils::calculatePositionsGrid2D(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue) {	
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	int totalParticles = rangeMax - rangeMin;
	if (totalParticles <= 0) return;

	int gridX = std::ceil(std::sqrt(totalParticles));
	int gridZ = gridX;

	glm::vec3 stepSize = (maxValue - minValue) / glm::vec3(gridX - 1, 1, gridZ - 1);

	int count = 0;

	for (int x = 0; x < gridX && count < totalParticles; x++) {
		for (int z = 0; z < gridZ && count < totalParticles; z++) {
			int index = rangeMin + count;
			glm::vec3 position = minValue + glm::vec3(x * stepSize.x, 0, z * stepSize.z);
			r_particles[index].setPosition(position);
			count++;

			if (count >= totalParticles)
				return;
		}
	}
}

// Initialize the particles' velocities randomly between minValue
// and maxValue from rangeMin to rangeMax in the list
void PresetUtils::calculateVelocitiesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setVelocity(glm::vec3(randomFloat(minValue.x, maxValue.x), randomFloat(minValue.y, maxValue.y), randomFloat(minValue.z, maxValue.z)));
	}
}

// Initialize the particles' velocities to orbit a
// center mass from rangeMin to rangeMax in the list
void PresetUtils::calculateVelocitiesOrbit(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec4 center, float velocityScale) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();
	
	for (int i = rangeMin; i < rangeMax; i++)
	{
		glm::vec3 position = r_particles[i].getPosition();
		float px = position.x;
		float py = position.y;
		float pz = position.z;

		// Calculate the distance from the center to the particle
		float distance = sqrt((px - center.x) * (px - center.x) + (py - center.y) * (py - center.y) + (pz - center.z) * (pz - center.z));

		// Calculate initial velocity magnitude for circular rotation, based on distance
		float velocityMagnitude = 0.f;
		if (distance > 0.f)
			velocityMagnitude = sqrt(center.w / distance) * velocityScale;

		// Perpendicular vector for the initial velocity
		float rx = px - center.x;
		float ry = py - center.y;
		float rz = pz - center.z;

		// Consistent reference vector for the cross product
		float ref_x = 0.0f;
		float ref_y = 1.0f;
		float ref_z = 0.0f;

		// Cross product of radius vector with reference vector
		float vx = ry * ref_z - rz * ref_y;
		float vy = rz * ref_x - rx * ref_z;
		float vz = rx * ref_y - ry * ref_x;

		// Normalize the perpendicular vector
		float length = sqrt(vx * vx + vy * vy + vz * vz);
		if (length > 0.00001f) {
			vx = (vx / length) * velocityMagnitude;
			vy = (vy / length) * velocityMagnitude;
			vz = (vz / length) * velocityMagnitude;
		}

		// Set velocity
		r_particles[i].setVelocity(glm::vec3(vx, vy, vz));
	}
}

// Initialize the particles' masses with a constant
// value from rangeMin to rangeMax in the list
void PresetUtils::calculateMassesConstant(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float value) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setMass(value);
	}
}

// Initialize the particles' masses randomly between minValue
// and maxValue from rangeMin to rangeMax in the list
void PresetUtils::calculateMassesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float minValue, float maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setMass(randomFloat(minValue, maxValue));
	}
}

// Initialize the particles' sizes with a constant
// value from rangeMin to rangeMax in the list
void PresetUtils::calculateSizesConstant(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float value) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setSize(value);
	}
}

// Initialize the particles' sizes randomly between minValue
// and maxValue from rangeMin to rangeMax in the list
void PresetUtils::calculateSizesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float minValue, float maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setSize(randomFloat(minValue, maxValue));
	}
}