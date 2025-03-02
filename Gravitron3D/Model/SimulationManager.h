#pragma once

#include <vector>
#include "Particle.h"
#include "Octree.h"
#include "Settings.h"
#include "PresetUtils.h"
#include "PresetType.h"
#include "PositionType.h"
#include "VelocityType.h"
#include "../Persistence/ParticleDataLoader.h"
#include "../Persistence/SettingsDataLoader.h"

struct SUpdateInfo
{
	float ElapsedTimeInSec = 0.0f;
	float DeltaTimeInSec = 0.0f;
};

class SimulationManager {
public:
	Settings settings;
	std::vector<Particle> particles;
	Octree octree;

	SimulationManager() { };

	void initSimulation(uint32_t numberOfParticles, PresetType preset, PositionType position, VelocityType velocity);
	void updateSimulation(const SUpdateInfo& updateInfo);

private:
	void updateParticles(float deltaTime);
	void updateParticlesRange(size_t start, size_t end, float deltaTime);
};