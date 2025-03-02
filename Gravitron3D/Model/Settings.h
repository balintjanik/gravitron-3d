#pragma once

#include <string>

struct Settings {
	int numberOfParticles;
	float simulationSpeed;

	Settings() : numberOfParticles(0), simulationSpeed(0) {};
};