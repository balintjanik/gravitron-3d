#include "SimulationManager.h"
#include <thread>

void SimulationManager::initSimulation(uint32_t numberOfParticles, PresetType preset, PositionType position, VelocityType velocity) {
	settings = Settings(numberOfParticles);
	settings.setNumberOfThreads(std::thread::hardware_concurrency());
	if (settings.getNumberOfThreads() == 0) settings.setNumberOfThreads(4); // Fallback to 4 if hardware_concurrency cannot detect

	particles.clear();
	particles = PresetUtils::generateParticles(numberOfParticles, preset, position, velocity);
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

void SimulationManager::updateParticlesRange(size_t start, size_t end, float deltaTime) {
	for (size_t i = start; i < end; i++) {
		if (i == 0) continue; //  TODO: remove this to allow center particle to move

		Particle& p = particles[i];
		glm::vec3 acceleration = glm::vec3(0);
		float allForce = octree.calculateAcceleration(acceleration, p.getPosition(), settings.getTheta(), settings.getEpsilon());

		p.setForce(allForce);
		p.setAcceleration(acceleration);
		p.setVelocity(p.getVelocity() + p.getAcceleration() * deltaTime * settings.getSimulationSpeed());
		p.setPosition(p.getPosition() + p.getVelocity() * deltaTime * settings.getSimulationSpeed());
	}
}

void SimulationManager::updateParticles(float deltaTime) {
	std::vector<std::thread> threads;
	size_t chunk_size = settings.getNumberOfParticles() / settings.getNumberOfThreads();

	for (size_t t = 0; t < settings.getNumberOfThreads(); ++t) {
		size_t start = t * chunk_size;
		size_t end = (t == settings.getNumberOfThreads() - 1) ? settings.getNumberOfParticles() : (t + 1) * chunk_size;

		threads.emplace_back(&SimulationManager::updateParticlesRange, this, start, end, deltaTime);
	}

	// Join all threads
	for (std::thread& thread : threads) {
		thread.join();
	}
}

void SimulationManager::loadSettings(const std::string& filename) {
	settings = SettingsDataLoader::loadFromFile(filename);
}

void SimulationManager::saveSettings(const std::string& filename) {
	SettingsDataLoader::saveToFile(filename, settings);
}