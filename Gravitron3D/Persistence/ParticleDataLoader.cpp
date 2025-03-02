#include "ParticleDataLoader.h"
#include <fstream>
#include <iostream>

void ParticleDataLoader::saveImpl(const std::string& filename, const std::vector<Particle>& particles) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }

    file << particles.size() << "\n";
    for (const auto& p : particles) {
        file << p.getPosition().x << " " << p.getPosition().y << " " << p.getPosition().z << " " << p.getMass() << " "
            << p.getVelocity().x << " " << p.getVelocity().y << " " << p.getVelocity().z << " " << p.getSize() << " "
            << p.getAcceleration().x << " " << p.getAcceleration().y << " " << p.getAcceleration().z << " " << p.getForce() << "\n";
    }
}

void ParticleDataLoader::loadImpl(const std::string& filename, std::vector<Particle>& particles) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file for reading: " << filename << std::endl;
        return;
    }

    size_t count;
    file >> count;
    particles.resize(count);

    for (auto& p : particles) {
        glm::vec3 position, velocity, acceleration;
        float mass, size, force;

        file >> position.x >> position.y >> position.z >> mass
            >> velocity.x >> velocity.y >> velocity.z >> size
            >> acceleration.x >> acceleration.y >> acceleration.z >> force;

        p.setPosition(position);
        p.setMass(mass);
        p.setVelocity(velocity);
        p.setSize(size);
        p.setAcceleration(acceleration);
        p.setForce(force);
    }
}