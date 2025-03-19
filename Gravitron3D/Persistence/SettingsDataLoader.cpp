#include "SettingsDataLoader.h"
#include <fstream>
#include <iostream>

void SettingsDataLoader::saveToFile(const std::string& filename, const Settings& settings) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open file for saving: " + filename);
    }

    // Save version
    int version = settings.getVersion();
    out.write(reinterpret_cast<const char*>(&version), sizeof(int));

    // Save other members via getters
    uint32_t numberOfParticles = settings.getNumberOfParticles();
    out.write(reinterpret_cast<const char*>(&numberOfParticles), sizeof(uint32_t));

    float simulationSpeed = settings.getSimulationSpeed();
    out.write(reinterpret_cast<const char*>(&simulationSpeed), sizeof(float));

    uint32_t numberOfThreads = settings.getNumberOfThreads();
    out.write(reinterpret_cast<const char*>(&numberOfThreads), sizeof(uint32_t));

    float theta = settings.getTheta();
    out.write(reinterpret_cast<const char*>(&theta), sizeof(float));

    float epsilon = settings.getEpsilon();
    out.write(reinterpret_cast<const char*>(&epsilon), sizeof(float));

    glm::vec4 lightPos = settings.getLightPos();
    out.write(reinterpret_cast<const char*>(&lightPos), sizeof(glm::vec4));

    float lightConstantAttenuation = settings.getLightConstantAttenuation();
    out.write(reinterpret_cast<const char*>(&lightConstantAttenuation), sizeof(float));

    float lightLinearAttenuation = settings.getLightLinearAttenuation();
    out.write(reinterpret_cast<const char*>(&lightLinearAttenuation), sizeof(float));

    float lightQuadraticAttenuation = settings.getLightQuadraticAttenuation();
    out.write(reinterpret_cast<const char*>(&lightQuadraticAttenuation), sizeof(float));

    float scaleFactor = settings.getScaleFactor();
    out.write(reinterpret_cast<const char*>(&scaleFactor), sizeof(float));

    if (!out) {
        throw std::runtime_error("Failed to write settings data.");
    }

    out.close();
}

Settings SettingsDataLoader::loadFromFile(const std::string& filename) {
    Settings settings;
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open file for loading: " + filename);
    }

    // Read version first and check it
    int version;
    in.read(reinterpret_cast<char*>(&version), sizeof(int));
    if (version != CURRENT_SETTINGS_VERSION) {
        throw std::runtime_error("Settings version mismatch. Expected version: " + std::to_string(CURRENT_SETTINGS_VERSION) + ", found: " + std::to_string(version));
    }

    // Read other settings data
    uint32_t numberOfParticles;
    in.read(reinterpret_cast<char*>(&numberOfParticles), sizeof(uint32_t));
    settings.setNumberOfParticles(numberOfParticles);

    float simulationSpeed;
    in.read(reinterpret_cast<char*>(&simulationSpeed), sizeof(float));
    settings.setSimulationSpeed(simulationSpeed);

    uint32_t numberOfThreads;
    in.read(reinterpret_cast<char*>(&numberOfThreads), sizeof(uint32_t));
    settings.setNumberOfThreads(numberOfThreads);

    float theta;
    in.read(reinterpret_cast<char*>(&theta), sizeof(float));
    settings.setTheta(theta);

    float epsilon;
    in.read(reinterpret_cast<char*>(&epsilon), sizeof(float));
    settings.setEpsilon(epsilon);

    glm::vec4 lightPos;
    in.read(reinterpret_cast<char*>(&lightPos), sizeof(glm::vec4));
    settings.setLightPos(lightPos);

    float lightConstantAttenuation;
    in.read(reinterpret_cast<char*>(&lightConstantAttenuation), sizeof(float));
    settings.setLightConstantAttenuation(lightConstantAttenuation);

    float lightLinearAttenuation;
    in.read(reinterpret_cast<char*>(&lightLinearAttenuation), sizeof(float));
    settings.setLightLinearAttenuation(lightLinearAttenuation);

    float lightQuadraticAttenuation;
    in.read(reinterpret_cast<char*>(&lightQuadraticAttenuation), sizeof(float));
    settings.setLightQuadraticAttenuation(lightQuadraticAttenuation);

    float scaleFactor;
    in.read(reinterpret_cast<char*>(&scaleFactor), sizeof(float));
    settings.setScaleFactor(scaleFactor);

    if (!in) {
        throw std::runtime_error("Failed to read settings data.");
    }

    in.close();
    return settings;
}