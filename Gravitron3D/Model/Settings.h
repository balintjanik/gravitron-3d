#pragma once

#include "glm/glm.hpp"
#include <string>

struct Settings {
private:
	uint32_t numberOfParticles;

	float simulationSpeed;
    static constexpr float minSimulationSpeed = 0.0f;
    static constexpr float maxSimulationSpeed = 5.0f;

	uint32_t numberOfThreads = 1;

	float theta = 1.0f;
    static constexpr float minTheta = 0.0f;
    static constexpr float maxTheta = 2.0f;
	float epsilon = 1.0f;
    static constexpr float minEpsilon = 0.0f;
    static constexpr float maxEpsilon = 2.0f;

    glm::vec4 lightPos = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    float lightConstantAttenuation = 0.0;
    static constexpr float minLightConstantAttenuation = 0.0f;
    static constexpr float maxLightConstantAttenuation = 1.0f;

    float lightLinearAttenuation = 1.0;
    static constexpr float minLightLinearAttenuation = 0.0f;
    static constexpr float maxLightLinearAttenuation = 1.0f;

    float lightQuadraticAttenuation = 0.0;
    static constexpr float minLightQuadraticAttenuation = 0.0f;
    static constexpr float maxLightQuadraticAttenuation = 1.0f;

    float scaleFactor = 0.005f;
    static constexpr float minScaleFactor = 0.001f;
    static constexpr float maxScaleFactor = 0.1f;

    static constexpr float WORLD_BOUNDS_MIN = -3000.f;
    static constexpr float WORLD_BOUNDS_MAX =  3000.f;

public:
	Settings(uint32_t _numberOfParticles = 0, float _simulationSpeed = 0.5f) : numberOfParticles(_numberOfParticles), simulationSpeed(_simulationSpeed) {};

    // Getters
    uint32_t getNumberOfParticles() const { return numberOfParticles; }

    float getSimulationSpeed() const { return simulationSpeed; }
    float getMinSimulationSpeed() const { return minSimulationSpeed; }
    float getMaxSimulationSpeed() const { return maxSimulationSpeed; }

    uint32_t getNumberOfThreads() const { return numberOfThreads; }

    float getTheta() const { return theta; }
    float getMinTheta() const { return minTheta; }
    float getMaxTheta() const { return maxTheta; }

    float getEpsilon() const { return epsilon; }
    float getMinEpsilon() const { return minEpsilon; }
    float getMaxEpsilon() const { return maxEpsilon; }

    glm::vec4 getLightPos() const { return lightPos; }

    float getLightConstantAttenuation() const { return lightConstantAttenuation; }
    float getMinLightConstantAttenuation() const { return minLightConstantAttenuation; }
    float getMaxLightConstantAttenuation() const { return maxLightConstantAttenuation; }

    float getLightLinearAttenuation() const { return lightLinearAttenuation; }
    float getMinLightLinearAttenuation() const { return minLightLinearAttenuation; }
    float getMaxLightLinearAttenuation() const { return maxLightLinearAttenuation; }

    float getLightQuadraticAttenuation() const { return lightQuadraticAttenuation; }
    float getMinLightQuadraticAttenuation() const { return minLightQuadraticAttenuation; }
    float getMaxLightQuadraticAttenuation() const { return maxLightQuadraticAttenuation; }

    float getScaleFactor() const { return scaleFactor; }
    float getMinScaleFactor() const { return minScaleFactor; }
    float getMaxScaleFactor() const { return maxScaleFactor; }

    float getMinWorldBounds() const { return WORLD_BOUNDS_MIN; }
    float getMaxWorldBounds() const { return WORLD_BOUNDS_MAX; }

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
        theta = glm::clamp(_theta, minTheta, maxTheta);
    }
    void setEpsilon(float _epsilon) {
        epsilon = glm::clamp(_epsilon, minEpsilon, maxEpsilon);
    }
    void setLightPos(const glm::vec4& pos) { lightPos = pos; }
    void setLightConstantAttenuation(float constant) {
        lightConstantAttenuation = glm::clamp(constant, minLightConstantAttenuation, maxLightConstantAttenuation);
    }
    void setLightLinearAttenuation(float linear) {
        lightLinearAttenuation = glm::clamp(linear, minLightLinearAttenuation, maxLightLinearAttenuation);
    }
    void setLightQuadraticAttenuation(float quadratic) {
        lightQuadraticAttenuation = glm::clamp(quadratic, minLightQuadraticAttenuation, maxLightQuadraticAttenuation);
    }
    void setScaleFactor(float scale) {
        scaleFactor = glm::clamp(scale, minScaleFactor, maxScaleFactor);
    }
};