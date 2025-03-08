#include "ParticleDataLoader.h"
#include <fstream>
#include <iostream>

void ParticleDataLoader::saveToFile(const std::string& filename, const std::vector<Particle>& particles) {
    std::cout << "Saving particle data to " << filename << std::endl;
    
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    // Write the number of particles first
    size_t count = particles.size();
    outFile.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

    // Write all particle data
    for (const auto& particle : particles) {
        outFile.write(reinterpret_cast<const char*>(&particle), sizeof(Particle));
    }

    outFile.close();
}

std::vector<Particle> ParticleDataLoader::loadFromFile(const std::string& filename) {
    std::cout << "Loading particle data from " << filename << std::endl;
    
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return {};
    }

    // Read the number of particles
    size_t count;
    inFile.read(reinterpret_cast<char*>(&count), sizeof(size_t));

    std::vector<Particle> particles(count);

    // Read all particle data
    inFile.read(reinterpret_cast<char*>(particles.data()), count * sizeof(Particle));

    inFile.close();
    return particles;
}