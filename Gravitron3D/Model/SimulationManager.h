#pragma once

#include <vector>
#include "SUpdateInfo.h"
#include "Particle.h"
#include "Octree.h"
#include "Settings.h"
#include "PresetUtils.h"
#include "PresetType.h"
#include "PositionType.h"
#include "VelocityType.h"
#include "../Persistence/ParticleDataLoader.h"
#include "../Persistence/SettingsDataLoader.h"

class SimulationManager {
public:
	Settings settings;
	std::vector<Particle> particles;
	Octree octree;

	SimulationManager() { };

	void initSimulation(uint32_t numberOfParticles, PresetType preset, PositionType position, VelocityType velocity);
	void updateSimulation(const SUpdateInfo& updateInfo);

	void addParticle(glm::vec4 positionMass, glm::vec4 velocitySize, glm::vec4 accelerationForce);

	void loadSettings(const std::string& filename);
	void saveSettings(const std::string& filename);
	void defaultSettings();

	void loadParticles(const std::string& filename);
	void saveParticles(const std::string& filename);

	float getMinWorldBound() const { return minWorldBound; }
	float getMaxWorldBound() const { return maxWorldBound; }

private:
	const float minWorldBound = -10000.0f;
	const float maxWorldBound =  10000.0f;

	void updateParticles(float deltaTime);
	void updateParticlesRange(size_t start, size_t end, float deltaTime);
	void handleWorldBounds(glm::vec3& r_position, glm::vec3& r_velocity);
};