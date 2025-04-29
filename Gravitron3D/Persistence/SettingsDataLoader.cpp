#include "SettingsDataLoader.h"
#include <fstream>
#include <iostream>
#include <vector>

// Save the provided settings to a .stg file
void SettingsDataLoader::saveToFile(const std::string& filename, const Settings& settings) {
    // Open file
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Failed to open file for saving: " + filename);
    }

    // Save version
    out << "version=" << settings.getVersion() << std::endl;

    // Save all attributes
    out << "numberOfParticles=" << settings.getNumberOfParticles() << std::endl;
    out << "simulationSpeed=" << settings.getSimulationSpeed() << std::endl;
    out << "numberOfThreads=" << settings.getNumberOfThreads() << std::endl;
    out << "theta=" << settings.getTheta() << std::endl;
    out << "epsilon=" << settings.getEpsilon() << std::endl;

    glm::vec4 lightPos = settings.getLightPos();
    out << "lightPos=" << lightPos.x << "," << lightPos.y << "," << lightPos.z << "," << lightPos.w << std::endl;

    out << "lightConstantAttenuation=" << settings.getLightConstantAttenuation() << std::endl;
    out << "lightLinearAttenuation=" << settings.getLightLinearAttenuation() << std::endl;
    out << "lightQuadraticAttenuation=" << settings.getLightQuadraticAttenuation() << std::endl;
    out << "scaleFactor=" << settings.getScaleFactor() << std::endl;

    out << "isForceColor=" << settings.getIsForceColor() << std::endl;
    out << "minForceColor=" << settings.getMinForceColor() << std::endl;
    out << "maxForceColor=" << settings.getMaxForceColor() << std::endl;
    glm::vec3 backgroundColor = settings.getBackgroundColor();
    out << "backgroundColor=" << backgroundColor.x << "," << backgroundColor.y << "," << backgroundColor.z << std::endl;

    if (!out) {
        throw std::runtime_error("Failed to write settings data.");
    }

    // Close file
    out.close();
}

// Load settings data from a .stg file into an object
Settings SettingsDataLoader::loadFromFile(const std::string& filename) {
    // Open file
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Failed to open file for loading: " + filename);
    }

    Settings settings;
    const std::vector<std::string> keys = {
        "version", "numberOfParticles", "simulationSpeed", "numberOfThreads", "theta", "epsilon",
        "lightPos", "lightConstantAttenuation", "lightLinearAttenuation", "lightQuadraticAttenuation", "scaleFactor",
        "isForceColor", "minForceColor", "maxForceColor", "backgroundColor"
    };

    // Read settings data
    int index = 0;
    const int maxIndex = keys.size();
    std::string line;
    while (std::getline(in, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        // Skip invalid lines
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        // Read key-value pairs
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Parse value
        if (key == keys[index] && index == 0) {
            int version = std::stoi(value);
            if (version != CURRENT_SETTINGS_VERSION) {
                throw std::runtime_error("Settings version mismatch. Expected version: " + std::to_string(CURRENT_SETTINGS_VERSION) + ", found: " + std::to_string(version));
            }
        }
        else if (key == keys[index] && index == 1) {
            settings.setNumberOfParticles(std::stoi(value));
        }
        else if (key == keys[index] && index == 2) {
            settings.setSimulationSpeed(std::stof(value));
        }
        else if (key == keys[index] && index == 3) {
            settings.setNumberOfThreads(std::stoi(value));
        }
        else if (key == keys[index] && index == 4) {
            settings.setTheta(std::stof(value));
        }
        else if (key == keys[index] && index == 5) {
            settings.setEpsilon(std::stof(value));
        }
        else if (key == keys[index] && index == 6) {
            glm::vec4 lightPos;
            std::sscanf(value.c_str(), "%f,%f,%f,%f", &lightPos.x, &lightPos.y, &lightPos.z, &lightPos.w);
            settings.setLightPos(lightPos);
        }
        else if (key == keys[index] && index == 7) {
            settings.setLightConstantAttenuation(std::stof(value));
        }
        else if (key == keys[index] && index == 8) {
            settings.setLightLinearAttenuation(std::stof(value));
        }
        else if (key == keys[index] && index == 9) {
            settings.setLightQuadraticAttenuation(std::stof(value));
        }
        else if (key == keys[index] && index == 10) {
            settings.setScaleFactor(std::stof(value));
        }
        else if (key == keys[index] && index == 11) {
            settings.setIsForceColor(std::stof(value));
        }
        else if (key == keys[index] && index == 12) {
            settings.setMinForceColor(std::stof(value));
        }
        else if (key == keys[index] && index == 13) {
            settings.setMaxForceColor(std::stof(value));
        }
        else if (key == keys[index] && index == 14) {
            glm::vec3 backgroundColor;
            std::sscanf(value.c_str(), "%f,%f,%f", &backgroundColor.x, &backgroundColor.y, &backgroundColor.z);
            settings.setBackgroundColor(backgroundColor);
        }
        else {
            if (std::find(keys.begin(), keys.end(), key) != keys.end() && index < maxIndex) {
                throw std::runtime_error("Missing data: " + keys[index]);
            }
            else if (index < maxIndex){
                throw std::runtime_error("Unrecognized key: " + key);
            }
        }

        index++;
    }

    if (index < maxIndex) {
        throw std::runtime_error("Not all settings could be read in");
    }
    else if (index > maxIndex) {
        throw std::runtime_error("Unexpected extra data found");
    }

    if (in.fail() && !in.eof()) {
        throw std::runtime_error("Failed to read settings data");
    }

    // Close file
    in.close();
    return settings;
}