#include "ParticleDataLoader.h"
#include <fstream>
#include <iostream>

void ParticleDataLoader::saveToFile(const std::string& filename, const std::vector<Particle>& particles) {
    std::cout << "Saving particle data to " << filename << std::endl;
    // TODO: add actual save logic here
}

std::vector<Particle> ParticleDataLoader::loadFromFile(const std::string& filename) {
    std::cout << "Loading particle data from " << filename << std::endl;
    std::vector<Particle> particles;
    // TODO: add actual load logic here
    return particles;
}