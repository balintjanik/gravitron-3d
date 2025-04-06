#include "PresetUtils.h"

float PresetUtils::randomFloat(float min, float max)
{
	// TODO: this assumes max>min
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random * range) + min;
}

std::vector<Particle> PresetUtils::generateParticles(int numberOfParticles, PresetType preset, PositionType position, VelocityType velocity, MassType mass) {
	std::vector<Particle> particles;

	if (numberOfParticles == 0) return particles;

	for (int i = 0; i < numberOfParticles; i++) {
		particles.push_back(Particle());
		particles[i].setMass(1.f);
	}

	switch (preset)
	{
	case PRESET_GALAXY:
		initPresetGalaxy(particles);
		break;
	case PRESET_SOLAR_SYSTEM:
		initPresetSolarSystem(particles);
		break;
	case PRESET_GALAXY_COLLISION:
		initPresetGalaxyCollision(particles);
		break;
	case PRESET_RANDOM:
		initPresetRandom(particles);
		break;
	case PRESET_CUSTOM:
		initParticlePositions(particles, position);
		initParticleVelocities(particles, velocity);
		initParticleMasses(particles, mass);
		break;
	default:
		break;
	}

	return particles;
}

void PresetUtils::initParticlePositions(std::vector<Particle>& r_particles, PositionType position) {
	// TODO: specify range?
	if (position == POSITION_RANDOM) {
		int rangeMin = 0;
		int rangeMax = r_particles.size();
		glm::vec3 min = glm::vec3(-500.f, -500.f, -500.f);
		glm::vec3 max = glm::vec3( 500.f,  500.f,  500.f);
		
		calculatePositionsRandom(r_particles, rangeMin, rangeMax, min, max);
	}
	// TODO: specify radius?
	else if (position == POSITION_SPHERE) {
		int rangeMin = 0;
		int rangeMax = r_particles.size();
		glm::vec3 center = glm::vec3(0.0f);
		float radiusMin = 0.0f;
		float radiusMax = 300.0f;

		calculatePositionsSphere(r_particles, rangeMin, rangeMax, center, radiusMin, radiusMax, false);
	}
	// TODO: specify radius?
	else if (position == POSITION_DISK) {
		int rangeMin = 0;
		int rangeMax = r_particles.size();
		glm::vec3 center = glm::vec3(0.0f);
		float radiusMin = 0.0f;
		float radiusMax = 300.0f;
		
		calculatePositionsSphere(r_particles, rangeMin, rangeMax, center, radiusMin, radiusMax, true);
	}
	else if (position == POSITION_GRID_3D) {
		int rangeMin = 0;
		int rangeMax = r_particles.size();
		glm::vec3 min = glm::vec3(-250.f, -250.f,-250.f);
		glm::vec3 max = glm::vec3(250.f, 250.f, 250.f);

		calculatePositionsGrid3D(r_particles, rangeMin, rangeMax, min, max);
	}
	else if (position == POSITION_GRID_2D) {
		int rangeMin = 0;
		int rangeMax = r_particles.size();
		glm::vec3 min = glm::vec3(-250.f, 0.f, -250.f);
		glm::vec3 max = glm::vec3(250.f, 0.f, 250.f);

		calculatePositionsGrid2D(r_particles, rangeMin, rangeMax, min, max);
	}
	else {
		throw "Invalid position type.";
	}
}

void PresetUtils::initParticleVelocities(std::vector<Particle>& r_particles, VelocityType velocity) {
	// TODO: specify range?
	if (velocity == VELOCITY_RANDOM) {
		glm::vec3 min = glm::vec3(-50.f, -50.f, -50.f);
		glm::vec3 max = glm::vec3(50.f, 50.f, 50.f);
		
		calculateVelocitiesRandom(r_particles, 0, r_particles.size(), min, max);
	}
	else if (velocity == VELOCITY_ZERO) {
		for (int i = 0; i < r_particles.size(); i++) {

			r_particles[i].setVelocity(glm::vec3(0.f));
		}
	}
	else if (velocity == VELOCITY_ORBIT) {
		glm::vec4 center = glm::vec4(0.f, 0.f, 0.f, r_particles.size());
		float velocityScale = 1.0f;

		calculateVelocitiesOrbit(r_particles, 0, r_particles.size(), center, velocityScale);
	}
	else {
		throw "Invalid velocity type.";
	}
}

void PresetUtils::initParticleMasses(std::vector<Particle>& r_particles, MassType mass) {
	if (mass == MASS_CONSTANT) {
		float value = 1.0f;
		int rangeMin = 0;
		int rangeMax = r_particles.size();

		calculateMassesConstant(r_particles, rangeMin, rangeMax, value);
	}
	else if (MASS_RANDOM) {
		float minValue = 0.5f;
		float maxValue = 2.0f;
		int rangeMin = 0;
		int rangeMax = r_particles.size();

		calculateMassesRandom(r_particles, rangeMin, rangeMax, minValue, maxValue);
	}
	else {
		throw "Invalid mass type.";
	}
}

void PresetUtils::initPresetGalaxy(std::vector<Particle>& r_particles) {
	float centerMass = 100000.0f;
	glm::vec4 center = glm::vec4(0.f, 0.f, 0.f, centerMass);
	float radiusMin = 20.0f;
	float radiusMax = 300.0f;
	float velocityScale = 0.8f;
	float massMin = 0.5f;
	float massMax = 2.0f;
	r_particles[0].setPosition(center);
	r_particles[0].setMass(centerMass);
	r_particles[0].setMovable(false);

	calculatePositionsSphere(r_particles, 1, r_particles.size(), center, radiusMin, radiusMax, false);
	calculateVelocitiesOrbit(r_particles, 1, r_particles.size(), center, velocityScale);
	calculateMassesRandom(r_particles, 1, r_particles.size(), massMin, massMax);
	for (auto& p : r_particles) {
		p.setSize(randomFloat(0.5f, 2.0f));
	}
	r_particles[0].setSize(10.0f);
}

void PresetUtils::initPresetSolarSystem(std::vector<Particle>& r_particles) {
	// TODO: implement
}

void PresetUtils::initPresetGalaxyCollision(std::vector<Particle>& r_particles) {
	// TODO: implement
}

void PresetUtils::initPresetRandom(std::vector<Particle>& r_particles) {
	int rangeMin = 0;
	int rangeMax = r_particles.size();
	
	// Position
	glm::vec3 min = glm::vec3(-500.f, -500.f, -500.f);
	glm::vec3 max = glm::vec3(500.f, 500.f, 500.f);
	calculatePositionsRandom(r_particles, rangeMin, rangeMax, min, max);

	// Velocity
	min = glm::vec3(-50.f, -50.f, -50.f);
	max = glm::vec3(50.f, 50.f, 50.f);
	calculateVelocitiesRandom(r_particles, 0, r_particles.size(), min, max);

}

void PresetUtils::calculatePositionsRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setPosition(glm::vec3(randomFloat(minValue.x, maxValue.x), randomFloat(minValue.y, maxValue.y), randomFloat(minValue.z, maxValue.z)));
	}
}

void PresetUtils::calculatePositionsSphere(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 center, float radiusMin, float radiusMax, bool is2D) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

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

		if (is2D) {
			px = center.x + radius * cos(theta);
			py = center.y;
			pz = center.z + radius * sin(theta);
		}

		r_particles[i].setPosition(glm::vec3(px, py, pz));
	}
}

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

void PresetUtils::calculateVelocitiesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, glm::vec3 minValue, glm::vec3 maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setVelocity(glm::vec3(randomFloat(minValue.x, maxValue.x), randomFloat(minValue.y, maxValue.y), randomFloat(minValue.z, maxValue.z)));
	}
}

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

		// Calculate the 3D distance from the center to the particle
		float distance = sqrt((px - center.x) * (px - center.x) + (py - center.y) * (py - center.y) + (pz - center.z) * (pz - center.z));

		// Calculate initial velocity magnitude for circular rotation, based on 3D distance
		float velocityMagnitude = 0.f;
		if (distance > 0.f)
			velocityMagnitude = sqrt(center.w / distance) * velocityScale;

		// Determine a perpendicular vector for the initial velocity
		// Here, we use a simple approach by crossing the radius vector with an arbitrary vector (1, 0, 0) to get a perpendicular direction
		float rx = px - center.x;
		float ry = py - center.y;
		float rz = pz - center.z;

		// Use a consistent reference vector for the cross product, e.g., (0, 1, 0)
		float ref_x = 0.0f;
		float ref_y = 1.0f;
		float ref_z = 0.0f;

		// Cross product of radius vector (rx, ry, rz) with reference vector (ref_x, ref_y, ref_z)
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

		// Add particle to the list
		r_particles[i].setVelocity(glm::vec3(vx, vy, vz));
	}
}

void PresetUtils::calculateMassesConstant(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float value) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setMass(value);
	}
}

void PresetUtils::calculateMassesRandom(std::vector<Particle>& r_particles, int rangeMin, int rangeMax, float minValue, float maxValue) {
	if (rangeMin < 0)
		rangeMin = 0;

	if (rangeMax > r_particles.size())
		rangeMax = r_particles.size();

	for (int i = rangeMin; i < rangeMax; i++) {

		r_particles[i].setMass(randomFloat(minValue, maxValue));
	}
}