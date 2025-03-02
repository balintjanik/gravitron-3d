#pragma once

#include "DataLoaderTemplate.h"
#include "../Model/Particle.h"
#include <vector>
#include <string>

class ParticleDataLoader : public DataLoader<ParticleDataLoader> {
public:
    static void saveImpl(const std::string& filename, const std::vector<Particle>& particles);
    static void loadImpl(const std::string& filename, std::vector<Particle>& particles);
};
