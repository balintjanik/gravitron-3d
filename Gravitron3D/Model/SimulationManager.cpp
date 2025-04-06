#include "SimulationManager.h"
#include <thread>
#include <stdexcept>

void SimulationManager::initSettings() {
	settings = Settings();
	settings.setNumberOfThreads(std::thread::hardware_concurrency());
	if (settings.getNumberOfThreads() == 0) settings.setNumberOfThreads(4); // Fallback to 4 if hardware_concurrency cannot detect
}

void SimulationManager::initSimulation(uint32_t numberOfParticles, PresetType preset, PositionType position, VelocityType velocity, MassType mass, SizeType size) {
	settings.setNumberOfParticles(numberOfParticles);

	particles.clear();
	particles = PresetUtils::generateParticles(numberOfParticles, preset, position, velocity, mass, size);
}

void SimulationManager::updateSimulation(const SUpdateInfo& updateInfo) {
	// Build tree
	Octant octant = Octant::createNewContaining(particles);
	octree.clear(octant);

	for (auto& p : particles) {
		octree.insert(p.getPosition(), p.getMass());
	}

	// Propagate
	octree.propagate();

	// Update particles
	updateParticles(updateInfo.DeltaTimeInSec);
}

void SimulationManager::addParticle(glm::vec4 positionMass, glm::vec4 velocitySize, glm::vec4 accelerationForce, glm::vec4 colorMovable) {
	particles.push_back(Particle(positionMass, velocitySize, accelerationForce, colorMovable));
	settings.setNumberOfParticles(particles.size());
}

void SimulationManager::addGroup(int numberOfParticlesToAdd,
	PositionType position,
	glm::vec3 cubeMin,
	glm::vec3 cubeMax,
	glm::vec3 sphereCenter,
	float sphereRadiusMin,
	float sphereRadiusMax,
	VelocityType velocity,
	float velocityScale,
	float centerMass,
	glm::vec3 velocityRandomMin,
	glm::vec3 velocityRandomMax,
	bool movable,
	glm::vec3 overallVelocity,
	MassType mass,
	float massValue,
	float massRandomMin,
	float massRandomMax,
	SizeType size,
	float sizeValue,
	float sizeRandomMin,
	float sizeRandomMax)
{
	int previousNumberOfParticles = settings.getNumberOfParticles();
	settings.setNumberOfParticles(previousNumberOfParticles + numberOfParticlesToAdd);

	// Initialize center
	Particle centerParticle = Particle();
	if (position == POSITION_RANDOM || position == POSITION_GRID_2D || position == POSITION_GRID_3D)
		centerParticle.setPosition((cubeMin + cubeMax) / 2.0f);
	else
		centerParticle.setPosition(sphereCenter);

	if (velocity == VELOCITY_ORBIT) {
		centerParticle.setMass(centerMass);
		centerParticle.setSize(10);
	}
	else {
		centerParticle.setMass(1.0f);
	}

	if (!movable) {
		centerParticle.setMovable(false);
	}
	else {
		centerParticle.setVelocity(overallVelocity);
	}

	particles.push_back(centerParticle);

	// Add particles
	for (int i = 1; i < numberOfParticlesToAdd; i++) {
		particles.push_back(Particle());
		particles[previousNumberOfParticles + i].setMass(1.f);
	}

	// Initialize positions
	int rangeMin = previousNumberOfParticles + 1;
	int rangeMax = particles.size();
	switch (position)
	{
	case POSITION_RANDOM:
		PresetUtils::calculatePositionsRandom(particles, rangeMin, rangeMax, cubeMin, cubeMax);
		break;
	case POSITION_SPHERE:
		PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, sphereCenter, sphereRadiusMin, sphereRadiusMax, false);
		break;
	case POSITION_DISK:
		PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, sphereCenter, sphereRadiusMin, sphereRadiusMax, true);
		break;
	case POSITION_GRID_3D:
		PresetUtils::calculatePositionsGrid3D(particles, rangeMin, rangeMax, cubeMin, cubeMax);
		break;
	case POSITION_GRID_2D:
		PresetUtils::calculatePositionsGrid2D(particles, rangeMin, rangeMax, cubeMin, cubeMax);
		break;
	default:
		throw "Invalid position type.";
		break;
	}

	// Initialize velocities
	switch (velocity)
	{
	case VELOCITY_RANDOM:
		PresetUtils::calculateVelocitiesRandom(particles, rangeMin, rangeMax, velocityRandomMin, velocityRandomMax);
		break;
	case VELOCITY_ORBIT:
		glm::vec4 center = glm::vec4(centerParticle.getPosition(), centerMass);
		PresetUtils::calculateVelocitiesOrbit(particles, rangeMin, rangeMax, center, velocityScale);
		break;
	default:
		break;
	}

	// Add group overall velocity
	if (movable) {
		for (int i = rangeMin; i < rangeMax; i++) {
			particles[i].setVelocity(particles[i].getVelocity() + overallVelocity);
		}
	}

	// Initialize masses
	switch (mass)
	{
	case MASS_CONSTANT:
		PresetUtils::calculateMassesConstant(particles, rangeMin, rangeMax, massValue);
		break;
	case MASS_RANDOM:
		PresetUtils::calculateMassesRandom(particles, rangeMin, rangeMax, massRandomMin, massRandomMax);
		break;
	default:
		break;
	}

	// Initialize sizes
	switch (size)
	{
	case SIZE_CONSTANT:
		PresetUtils::calculateSizesConstant(particles, rangeMin, rangeMax, sizeValue);
		break;
	case SIZE_RANDOM:
		PresetUtils::calculateSizesRandom(particles, rangeMin, rangeMax, sizeRandomMin, sizeRandomMax);
		break;
	default:
		break;
	}
}

void SimulationManager::updateParticlesRange(size_t start, size_t end, float deltaTime) {
	for (size_t i = start; i < end; i++) {
		Particle& p = particles[i];

		if (!p.getMovable())
			continue;

		glm::vec3 acceleration = glm::vec3(0);
		float allForce = octree.calculateAcceleration(acceleration, p.getPosition(), settings.getTheta(), settings.getEpsilon());

		p.setForce(allForce);
		p.setAcceleration(acceleration);

		glm::vec3 newVelocity = p.getVelocity() + p.getAcceleration() * deltaTime * settings.getSimulationSpeed();
		glm::vec3 newPosition = p.getPosition() + newVelocity * deltaTime * settings.getSimulationSpeed();
		handleWorldBounds(newPosition, newVelocity);

		p.setVelocity(newVelocity);
		p.setPosition(newPosition);
	}
}

void SimulationManager::handleWorldBounds(glm::vec3& r_position, glm::vec3& r_velocity) {
	if (r_position.x < minWorldBound) {
		r_velocity.x *= -1;
		r_position.x = minWorldBound;
	}
	else if (r_position.x > maxWorldBound) {
		r_velocity.x *= -1;
		r_position.x = maxWorldBound;
	}
	if (r_position.y < minWorldBound) {
		r_velocity.y *= -1;
		r_position.y = minWorldBound;
	}
	else if (r_position.y > maxWorldBound) {
		r_velocity.y *= -1;
		r_position.y = maxWorldBound;
	}
	if (r_position.z < minWorldBound) {
		r_velocity.z *= -1;
		r_position.z = minWorldBound;
	}
	else if (r_position.z > maxWorldBound) {
		r_velocity.z *= -1;
		r_position.z = maxWorldBound;
	}
}

void SimulationManager::updateParticles(float deltaTime) {
	std::vector<std::thread> threads;
	size_t numThreads = std::min(1 + settings.getNumberOfParticles() / 200, settings.getNumberOfThreads());
	size_t chunk_size = settings.getNumberOfParticles() / numThreads;

	for (size_t t = 0; t < numThreads; ++t) {
		size_t start = t * chunk_size;
		size_t end = (t == numThreads - 1) ? settings.getNumberOfParticles() : (t + 1) * chunk_size;

		threads.emplace_back(&SimulationManager::updateParticlesRange, this, start, end, deltaTime);
	}

	for (std::thread& thread : threads) {
		thread.join();
	}
}

void SimulationManager::loadSettings(const std::string& filename) {
	try {
		Settings newSettings = SettingsDataLoader::loadFromFile(filename);
		settings = std::move(newSettings);
		settings.setNumberOfParticles(particles.size());
	}
	catch (const std::exception& e) {
		throw std::runtime_error(std::string(e.what()));
	}
}

void SimulationManager::saveSettings(const std::string& filename) {
	try {
		SettingsDataLoader::saveToFile(filename, settings);
	}
	catch (const std::exception& e) {
		throw std::runtime_error(std::string(e.what()));
	}
}

void SimulationManager::defaultSettings() {
	settings = Settings(particles.size());
}

void SimulationManager::loadParticles(const std::string& filename) {
	try {
		std::vector<Particle> newParticles = ParticleDataLoader::loadFromFile(filename);
		particles = std::move(newParticles);
		settings.setNumberOfParticles(particles.size());
	}
	catch (const std::exception& e) {
		throw std::runtime_error(std::string(e.what()));
	}
}

void SimulationManager::saveParticles(const std::string& filename) {
	try {
		ParticleDataLoader::saveToFile(filename, particles);
	}
	catch (const std::exception& e) {
		throw std::runtime_error(std::string(e.what()));
	}
}