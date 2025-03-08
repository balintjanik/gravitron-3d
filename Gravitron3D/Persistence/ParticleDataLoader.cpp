#include "ParticleDataLoader.h"
#include <fstream>
#include <iostream>

void ParticleDataLoader::saveToFile(const std::string& filename, const std::vector<Particle>& particles) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Unable to open file for writing.");
    }

    // Write the number of particles first
    size_t count = particles.size();
    outFile.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
    if (!outFile) {
        throw std::runtime_error("Failed to write particle count.");
    }

    // Write all particle data
    for (const auto& particle : particles) {
        outFile.write(reinterpret_cast<const char*>(&particle), sizeof(Particle));
        if (!outFile) {
            throw std::runtime_error("Failed to write particle data.");
        }
    }

    outFile.close();
}

std::vector<Particle> ParticleDataLoader::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open file for reading.");
    }

    // Read the number of particles
    size_t count;
    inFile.read(reinterpret_cast<char*>(&count), sizeof(size_t));
    if (!inFile) {
        throw std::runtime_error("Failed to read particle count.");
    }

    std::vector<Particle> particles(count);

    // Read all particle data
    inFile.read(reinterpret_cast<char*>(particles.data()), count * sizeof(Particle));
    if (!inFile) {
        throw std::runtime_error("Failed to read particle data.");
    }

    inFile.close();
    return particles;
}