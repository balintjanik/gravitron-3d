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

	void loadSettings(const std::string& filename);
	void saveSettings(const std::string& filename);

	void loadParticles(const std::string& filename);
	void saveParticles(const std::string& filename);

private:
	void updateParticles(float deltaTime);
	void updateParticlesRange(size_t start, size_t end, float deltaTime);
};