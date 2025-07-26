#pragma once

#include <vector>
#include "Particle.h"
#include "Octree.h"
#include "Settings.h"
#include "PresetUtils.h"
#include "ParticleGroupConfig.h"
#include "../Persistence/ParticleDataLoader.h"
#include "../Persistence/SettingsDataLoader.h"

class SimulationManager {
public:
	Settings settings;
	std::vector<Particle> particles;
	Octree octree;

	SimulationManager() { };

	void initSettings();
	void initSimulation(PresetType preset);
	void updateSimulation(const float deltaTime);

	void addParticle(glm::vec4 positionMass, glm::vec4 velocitySize, glm::vec4 accelerationForce, glm::vec4 colorMovable);
	void addGroup(const ParticleGroupConfig& config);

	void loadSettings(const std::string& filename);
	void saveSettings(const std::string& filename);
	void defaultSettings();

	void loadParticles(const std::string& filename);
	void saveParticles(const std::string& filename);

	float getMinWorldBound() const { return minWorldBound; }
	float getMaxWorldBound() const { return maxWorldBound; }

	void setMinWorldBound(float _minWorldBound) {
		if (_minWorldBound < MIN_WORLD_BOUND)
			minWorldBound = MIN_WORLD_BOUND;
		else if (_minWorldBound >= maxWorldBound)
			minWorldBound = maxWorldBound - 10;
		else
			minWorldBound = _minWorldBound;
	}
	void setMaxWorldBound(float _maxWorldBound) {
		if (_maxWorldBound > MAX_WORLD_BOUND)
			maxWorldBound = MAX_WORLD_BOUND;
		else if (_maxWorldBound <= minWorldBound)
			maxWorldBound = minWorldBound + 10;
		else
			maxWorldBound = _maxWorldBound;
	}

private:
	float minWorldBound = -10000.0f;
	float maxWorldBound =  10000.0f;
	const float MIN_WORLD_BOUND = -100000.0f;
	const float MAX_WORLD_BOUND =  100000.0f;

	void updateParticles(const float deltaTime);
	void updateParticlesRange(const size_t start, const size_t end, const float deltaTime);
	void handleWorldBounds(glm::vec3& r_position, glm::vec3& r_velocity);
};