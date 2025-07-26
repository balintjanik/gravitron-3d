#pragma once

#include "../Model/Particle.h"
#include <string>

class ParticleDataLoader {
public:
    static void saveToFile(const std::string& filename, const std::vector<Particle>& particles);

    static std::vector<Particle> loadFromFile(const std::string& filename);
};
