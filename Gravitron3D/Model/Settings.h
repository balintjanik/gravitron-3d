#pragma once

#include <string>

struct Settings {
private:
	uint32_t numberOfParticles;
	float simulationSpeed;

	uint32_t numberOfThreads = 1;

	float theta = 1.0f;
    float thetaMax = 2.0f;
    float thetaMin = 0.0f;
	float epsilon = 1.0f;
    float epsilonMax = 2.0f;
    float epsilonMin = 0.0f;

public:
	Settings(uint32_t _numberOfParticles = 0, float _simulationSpeed = 0.5f) : numberOfParticles(_numberOfParticles), simulationSpeed(_simulationSpeed) {};

    // Getters
    uint32_t getNumberOfParticles() const { return numberOfParticles; }
    float getSimulationSpeed() const { return simulationSpeed; }
    uint32_t getNumberOfThreads() const { return numberOfThreads; }
    float getTheta() const { return theta; }
    float getThetaMax() const { return thetaMax; }
    float getThetaMin() const { return thetaMin; }
    float getEpsilonMax() const { return epsilonMax; }
    float getEpsilonMin() const { return epsilonMin; }
    float getEpsilon() const { return epsilon; }

    // Setters
    void setNumberOfParticles(uint32_t _numberOfParticles) { numberOfParticles = _numberOfParticles; }
    void setSimulationSpeed(float _simulationSpeed) { simulationSpeed = _simulationSpeed; }
    void setNumberOfThreads(uint32_t _numberOfThreads) {
        if (_numberOfThreads < 1) {
            numberOfThreads = 1;
        }
        else {
            numberOfThreads = _numberOfThreads;
        }
    }
    void setTheta(float _theta) {
        if (_theta < 0.0f) {
            theta = 0.0f;
        }
        else if (_theta > 2.0f) {
            theta = 2.0f;
        }
        else {
            theta = _theta;
        }
    }
    void setEpsilon(float _epsilon) {
        if (_epsilon < 0.1f) {
            epsilon = 0.1f;
        }
        else if (_epsilon > 2.0f) {
            epsilon = 2.0f;
        }
        else {
            epsilon = _epsilon;
        }
    }
};