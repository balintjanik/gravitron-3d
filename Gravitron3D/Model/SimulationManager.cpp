#include "SimulationManager.h"
#include <thread>
#include <stdexcept>

// Initialize settings
void SimulationManager::initSettings() {
	settings = Settings();
	settings.setNumberOfThreads(std::thread::hardware_concurrency());
	if (settings.getNumberOfThreads() == 0) settings.setNumberOfThreads(4); // Fallback to 4 if hardware_concurrency cannot detect
}

// Initialize simulation with specified preset
void SimulationManager::initSimulation(PresetType preset) {
	particles.clear();
	settings.setNumberOfParticles(0);
	
	try {
		switch (preset)
		{
		case PRESET_GALAXY:
			loadParticles("Presets/Galaxy.csv");
			break;
		case PRESET_SOLAR_SYSTEM:
			loadParticles("Presets/SolarSystem.csv");
			break;
		case PRESET_GALAXY_COLLISION:
			loadParticles("Presets/GalaxyCollision.csv");
			break;
		case PRESET_GALAXY_COLLISION2:
			loadParticles("Presets/GalaxyCollision2.csv");
			break;
		case PRESET_EMPTY:
			break;
		default:
			break;
		}
	}
	catch (const std::exception& e) {
		throw std::runtime_error(std::string(e.what()));
	}

	settings.setNumberOfParticles(particles.size());
}

// Update simulation by one step
void SimulationManager::updateSimulation(const float deltaTime) {
	if (settings.getSimulationSpeed() == 0.0f) return;

	// Build tree
	Octant octant = Octant::createNewContaining(particles);
	octree.clear(octant);

	for (auto& p : particles) {
		octree.insert(p.getPosition(), p.getMass());
	}

	// Propagate
	octree.propagate();

	// Update particles
	updateParticles(deltaTime);
}

// Add a particle to the list
void SimulationManager::addParticle(glm::vec4 positionMass, glm::vec4 velocitySize, glm::vec4 accelerationForce, glm::vec4 colorMovable) {
	particles.push_back(Particle(positionMass, velocitySize, accelerationForce, colorMovable));
	settings.setNumberOfParticles(particles.size());
}

// Add a group of particles to the list
void SimulationManager::addGroup(const ParticleGroupConfig& config)
{
	if (config.numberOfParticlesToAdd == 0) return;

	int previousNumberOfParticles = settings.getNumberOfParticles();
	settings.setNumberOfParticles(previousNumberOfParticles + config.numberOfParticlesToAdd);

	// Initialize center
	Particle centerParticle = Particle();
	if (config.position == POSITION_RANDOM || config.position == POSITION_GRID_2D || config.position == POSITION_GRID_3D)
		centerParticle.setPosition((config.region.cubeMin + config.region.cubeMax) / 2.0f);
	else
		centerParticle.setPosition(config.region.sphereCenter);

	if (config.velocity.type == VELOCITY_ORBIT) {
		centerParticle.setMass(config.centerMass);
		centerParticle.setSize(10);
	}
	else {
		centerParticle.setMass(1.0f);
	}

	if (!config.movable) {
		centerParticle.setMovable(false);
	}
	else {
		centerParticle.setVelocity(config.velocity.overallVelocity);
	}

	particles.push_back(centerParticle);

	// Add particles
	for (int i = 1; i < config.numberOfParticlesToAdd; i++) {
		particles.push_back(Particle());
		particles[previousNumberOfParticles + i].setMass(1.f);
	}

	// Initialize positions
	int rangeMin = previousNumberOfParticles + 1;
	int rangeMax = particles.size();
	switch (config.position)
	{
	case POSITION_RANDOM:
		PresetUtils::calculatePositionsRandom(particles, rangeMin, rangeMax, config.region.cubeMin, config.region.cubeMax);
		break;
	case POSITION_SPHERE:
		PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, config.region.sphereCenter, config.region.sphereRadiusMin, config.region.sphereRadiusMax, false);
		break;
	case POSITION_DISK:
		PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, config.region.sphereCenter, config.region.sphereRadiusMin, config.region.sphereRadiusMax, true);
		break;
	case POSITION_GRID_3D:
		PresetUtils::calculatePositionsGrid3D(particles, rangeMin, rangeMax, config.region.cubeMin, config.region.cubeMax);
		break;
	case POSITION_GRID_2D:
		PresetUtils::calculatePositionsGrid2D(particles, rangeMin, rangeMax, config.region.cubeMin, config.region.cubeMax);
		break;
	default:
		break;
	}

	// Initialize velocities
	switch (config.velocity.type)
	{
	case VELOCITY_RANDOM:
		PresetUtils::calculateVelocitiesRandom(particles, rangeMin, rangeMax, config.velocity.randomMin, config.velocity.randomMax);
		break;
	case VELOCITY_ORBIT:
		glm::vec4 center = glm::vec4(centerParticle.getPosition(), config.centerMass);
		PresetUtils::calculateVelocitiesOrbit(particles, rangeMin, rangeMax, center, config.velocity.scale);
		break;
	default:
		break;
	}

	// Add group overall velocity
	if (config.movable) {
		for (int i = rangeMin; i < rangeMax; i++) {
			particles[i].setVelocity(particles[i].getVelocity() + config.velocity.overallVelocity);
		}
	}

	// Initialize masses
	switch (config.mass.type)
	{
	case MASS_CONSTANT:
		PresetUtils::calculateMassesConstant(particles, rangeMin, rangeMax, config.mass.value);
		break;
	case MASS_RANDOM:
		PresetUtils::calculateMassesRandom(particles, rangeMin, rangeMax, config.mass.randomMin, config.mass.randomMax);
		break;
	default:
		break;
	}

	// Initialize sizes
	switch (config.size.type)
	{
	case SIZE_CONSTANT:
		PresetUtils::calculateSizesConstant(particles, rangeMin, rangeMax, config.size.value);
		break;
	case SIZE_RANDOM:
		PresetUtils::calculateSizesRandom(particles, rangeMin, rangeMax, config.size.randomMin, config.size.randomMax);
		break;
	default:
		break;
	}
}

// Update particles in a specified range
void SimulationManager::updateParticlesRange(const size_t start, const size_t end, const float deltaTime) {
	for (size_t i = start; i < end; i++) {
		Particle& p = particles[i];

		// Skip if particle is not movable
		if (!p.getMovable())
			continue;

		// Calculate acceleration and accumulate forces
		glm::vec3 acceleration = glm::vec3(0);
		float allForce = octree.calculateAcceleration(acceleration, p.getPosition(), settings.getTheta(), settings.getEpsilon());

		// Update force and acceleration
		p.setForce(allForce);
		p.setAcceleration(acceleration);

		// Handle world bounds
		glm::vec3 newVelocity = p.getVelocity() + p.getAcceleration() * deltaTime * settings.getSimulationSpeed();
		glm::vec3 newPosition = p.getPosition() + newVelocity * deltaTime * settings.getSimulationSpeed();
		handleWorldBounds(newPosition, newVelocity);

		// Update velocity and position with the verified data
		p.setVelocity(newVelocity);
		p.setPosition(newPosition);
	}
}

// Handle world bounds (particles placed back in and bounce back from the bounds)
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

// Update particles of the simulation
void SimulationManager::updateParticles(const float deltaTime) {
	// Calculate necessary number of threads
	std::vector<std::thread> threads;
	size_t numThreads = std::min(1 + settings.getNumberOfParticles() / 200, settings.getNumberOfThreads());
	size_t chunkSize = settings.getNumberOfParticles() / numThreads;

	// Start threads, each calculating a chunk of the particles
	for (size_t t = 0; t < numThreads; ++t) {
		size_t start = t * chunkSize;
		size_t end = (t == numThreads - 1) ? settings.getNumberOfParticles() : (t + 1) * chunkSize;

		threads.emplace_back(&SimulationManager::updateParticlesRange, this, start, end, deltaTime);
	}

	// Join threads
	for (std::thread& thread : threads) {
		thread.join();
	}
}

// Load settings
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

// Save settings
void SimulationManager::saveSettings(const std::string& filename) {
	try {
		SettingsDataLoader::saveToFile(filename, settings);
	}
	catch (const std::exception& e) {
		throw std::runtime_error(std::string(e.what()));
	}
}

// Set settings back to default
void SimulationManager::defaultSettings() {
	initSettings();
	settings.setNumberOfParticles(particles.size());
}

// Load particles
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

// Save particles
void SimulationManager::saveParticles(const std::string& filename) {
	try {
		ParticleDataLoader::saveToFile(filename, particles);
	}
	catch (const std::exception& e) {
		throw std::runtime_error(std::string(e.what()));
	}
}