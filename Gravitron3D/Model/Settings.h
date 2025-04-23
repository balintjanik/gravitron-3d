#pragma once

#include "glm/glm.hpp"
#include <string>

constexpr int CURRENT_SETTINGS_VERSION = 6;

struct Settings {
private:
    int version = CURRENT_SETTINGS_VERSION;

    uint32_t numberOfParticles = 0;
    static constexpr uint32_t minNumberOfParticles = 0;
    static constexpr uint32_t maxNumberOfParticles = 1000000;

    float simulationSpeed = 0.5f;
    static constexpr float minSimulationSpeed = 0.0f;
    static constexpr float maxSimulationSpeed = 50.0f;

    uint32_t numberOfThreads = 1;

    float theta = 0.5f;
    static constexpr float minTheta = 0.0f;
    static constexpr float maxTheta = 2.0f;
    float epsilon = 1.0f;
    static constexpr float minEpsilon = 0.0f;
    static constexpr float maxEpsilon = 15.0f;

    glm::vec4 lightPos = glm::vec4(0.0f, -1.0f, 0.0f, 0.5f);
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

    bool isForceColor = true;
    float minForceColor = 0.0f;
    float maxForceColor = 100.0f;

    glm::vec3 backgroundColor = glm::vec3(0.0f);

public:
    Settings() {};

    // Getters
    int getVersion() const { return version; }
    uint32_t getNumberOfParticles() const { return numberOfParticles; }
    uint32_t getMinNumberOfParticles() const { return minNumberOfParticles; }
    uint32_t getMaxNumberOfParticles() const { return maxNumberOfParticles; }

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

    bool getIsForceColor() const { return isForceColor; }
    float getMinForceColor() const { return minForceColor; }
    float getMaxForceColor() const { return maxForceColor; }
    glm::vec3 getBackgroundColor() const { return backgroundColor; }

    // Setters
    void setNumberOfParticles(uint32_t _numberOfParticles) { numberOfParticles = glm::clamp(_numberOfParticles, minNumberOfParticles, maxNumberOfParticles); }
    void setSimulationSpeed(float _simulationSpeed) { simulationSpeed = glm::clamp(_simulationSpeed, minSimulationSpeed, maxSimulationSpeed); }
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
    void setIsForceColor(int _isForceColor) { isForceColor = _isForceColor; }
    void setMinForceColor(float _minForceColor) {
        minForceColor = glm::clamp(_minForceColor, 0.0f, maxForceColor);
    }
    void setMaxForceColor(float _maxForceColor) {
        maxForceColor = glm::clamp(_maxForceColor, minForceColor, std::numeric_limits<float>::max());
    }
    void setBackgroundColor(glm::vec3 _backgroundColor) {
        backgroundColor = glm::clamp(_backgroundColor, 0.0f, 1.0f);
    }
};