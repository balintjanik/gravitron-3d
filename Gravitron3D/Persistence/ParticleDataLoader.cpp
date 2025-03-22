#include "ParticleDataLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

void ParticleDataLoader::saveToFile(const std::string& filename, const std::vector<Particle>& particles) {
    std::ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("Unable to open file for writing.");
    }

    try {
        std::string separator = ";";
        outFile << "PositionX" << separator << "PositionY" << separator << "PositionZ" << separator << "Mass" << separator
            << "VelocityX" << separator << "VelocityY" << separator << "VelocityZ" << separator << "Size" << separator
            << "AccelerationX" << separator << "AccelerationY" << separator << "AccelerationZ" << separator << "Force\n";

        if (!outFile) {
            throw std::runtime_error("Failed to write header to file.");
        }

        for (const auto& particle : particles) {
            outFile << particle.getPosition().x << separator
                << particle.getPosition().y << separator
                << particle.getPosition().z << separator
                << particle.getMass() << separator
                << particle.getVelocity().x << separator
                << particle.getVelocity().y << separator
                << particle.getVelocity().z << separator
                << particle.getSize() << separator
                << particle.getAcceleration().x << separator
                << particle.getAcceleration().y << separator
                << particle.getAcceleration().z << separator
                << particle.getForce() << "\n";

            if (!outFile) {
                throw std::runtime_error("Failed to write particle data to file.");
            }
        }

    }
    catch (const std::exception& e) {
        outFile.close();
        throw;
    }

    outFile.close();
}

std::vector<Particle> ParticleDataLoader::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        throw std::runtime_error("Unable to open file for reading.");
    }

    std::vector<Particle> particles;
    std::string separator = ";";
    std::string line;

    // Read and discard the header line
    if (!std::getline(inFile, line)) {
        throw std::runtime_error("Failed to read header from file.");
    }

    // Read particle data
    while (std::getline(inFile, line)) {
        std::istringstream lineStream(line);
        std::vector<float> values;
        std::string value;

        while (std::getline(lineStream, value, separator[0])) {
            values.push_back(std::stof(value));
        }

        if (values.size() != 12) {
            throw std::runtime_error("Incorrect number of values in line: " + line);
        }

        Particle particle;
        particle.setPosition(glm::vec3(values[0], values[1], values[2]));
        particle.setMass(values[3]);
        particle.setVelocity(glm::vec3(values[4], values[5], values[6]));
        particle.setSize(values[7]);
        particle.setAcceleration(glm::vec3(values[8], values[9], values[10]));
        particle.setForce(values[11]);

        particles.push_back(particle);
    }

    if (!inFile.eof() && inFile.fail()) {
        throw std::runtime_error("Error while reading file.");
    }

    inFile.close();
    return particles;
}