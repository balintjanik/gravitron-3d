#pragma once

#include "../Model/Particle.h"
#include <string>

class ParticleDataLoader {
public:
    static void ParticleDataLoader::saveToFile(const std::string& filename, const std::vector<Particle>& particles);

    static std::vector<Particle> ParticleDataLoader::loadFromFile(const std::string& filename);
};
