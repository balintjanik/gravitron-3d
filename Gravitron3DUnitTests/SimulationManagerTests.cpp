#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/SimulationManager.h"

#include <fstream>
#include <string>
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(SimulationManagerTests)
    {
    public:
        const float tolerance = 0.0001f;

        inline void AssertVec4Equal(const glm::vec4& expected, const glm::vec4& actual) {
            Assert::AreEqual(expected.x, actual.x, tolerance);
            Assert::AreEqual(expected.y, actual.y, tolerance);
            Assert::AreEqual(expected.z, actual.z, tolerance);
            Assert::AreEqual(expected.w, actual.w, tolerance);
        }

        inline void AssertParticleFromLine(const std::string& line, const Particle& expected) {
            std::stringstream ss(line);
            std::string value;
            float components[16];
            int i = 0;

            while (std::getline(ss, value, ';') && i < 16) {
                components[i++] = std::stof(value);
            }

            Assert::AreEqual(16, i);

            glm::vec4 positionMass(components[0], components[1], components[2], components[3]);
            glm::vec4 velocitySize(components[4], components[5], components[6], components[7]);
            glm::vec4 accelerationForce(components[8], components[9], components[10], components[11]);
            glm::vec4 colorMovable(components[12], components[13], components[14], components[15]);

            AssertVec4Equal(glm::vec4(expected.getPosition(), expected.getMass()), positionMass);
            AssertVec4Equal(glm::vec4(expected.getVelocity(), expected.getSize()), velocitySize);
            AssertVec4Equal(glm::vec4(expected.getAcceleration(), expected.getForce()), accelerationForce);
            AssertVec4Equal(glm::vec4(expected.getColor(), expected.getMovable() ? 1.0f : 0.0f), colorMovable);
        }
        TEST_METHOD(InitSettings_InitializesCorrectly)
        {
            Settings defaultSettings;
            SimulationManager simulationManager;
            simulationManager.settings.setTheta(0.0f);
            simulationManager.settings.setNumberOfParticles(500);
            simulationManager.settings.setScaleFactor(0.075);

            simulationManager.initSettings();

            Assert::AreEqual(defaultSettings.getTheta(), simulationManager.settings.getTheta(), tolerance);
            Assert::AreEqual(defaultSettings.getNumberOfParticles(), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(defaultSettings.getScaleFactor(), simulationManager.settings.getScaleFactor(), tolerance);
        }

        TEST_METHOD(InitSimulation_PresetEmpty)
        {
            SimulationManager simulationManager;
            simulationManager.particles.push_back(Particle());

            simulationManager.initSimulation(PresetType::PRESET_EMPTY);

            Assert::AreEqual(static_cast<uint32_t>(0), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(static_cast<size_t>(0), simulationManager.particles.size());
        }

        TEST_METHOD(InitSimulation_PresetGalaxy)
        {
            SimulationManager simulationManager;
            simulationManager.particles.push_back(Particle());

            simulationManager.initSimulation(PresetType::PRESET_GALAXY);

            Assert::AreEqual(static_cast<uint32_t>(25000), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(static_cast<size_t>(25000), simulationManager.particles.size());

            std::ifstream in("Presets/Galaxy.csv");
            Assert::IsTrue(in.good(), L"File could not be opened");
            std::string line;
            std::getline(in, line);
            const std::string expectedHeader = "PositionX;PositionY;PositionZ;Mass;VelocityX;VelocityY;VelocityZ;Size;AccelerationX;AccelerationY;AccelerationZ;Force;ColorR;ColorG;ColorB;Movable";
            Assert::AreEqual(expectedHeader, line);
            for (Particle& particle : simulationManager.particles) {
                std::getline(in, line);
                AssertParticleFromLine(line, particle);
            }
        }

        TEST_METHOD(InitSimulation_PresetGalaxyCollision)
        {
            SimulationManager simulationManager;
            simulationManager.particles.push_back(Particle());

            simulationManager.initSimulation(PresetType::PRESET_GALAXY_COLLISION);

            Assert::AreEqual(static_cast<uint32_t>(30000), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(static_cast<size_t>(30000), simulationManager.particles.size());

            std::ifstream in("Presets/GalaxyCollision.csv");
            Assert::IsTrue(in.good(), L"File could not be opened");
            std::string line;
            std::getline(in, line);
            const std::string expectedHeader = "PositionX;PositionY;PositionZ;Mass;VelocityX;VelocityY;VelocityZ;Size;AccelerationX;AccelerationY;AccelerationZ;Force;ColorR;ColorG;ColorB;Movable";
            Assert::AreEqual(expectedHeader, line);
            for (Particle& particle : simulationManager.particles) {
                std::getline(in, line);
                AssertParticleFromLine(line, particle);
            }
        }

        TEST_METHOD(InitSimulation_PresetGalaxyCollision2)
        {
            SimulationManager simulationManager;
            simulationManager.particles.push_back(Particle());

            simulationManager.initSimulation(PresetType::PRESET_GALAXY_COLLISION2);

            Assert::AreEqual(static_cast<uint32_t>(30000), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(static_cast<size_t>(30000), simulationManager.particles.size());

            std::ifstream in("Presets/GalaxyCollision2.csv");
            Assert::IsTrue(in.good(), L"File could not be opened");
            std::string line;
            std::getline(in, line);
            const std::string expectedHeader = "PositionX;PositionY;PositionZ;Mass;VelocityX;VelocityY;VelocityZ;Size;AccelerationX;AccelerationY;AccelerationZ;Force;ColorR;ColorG;ColorB;Movable";
            Assert::AreEqual(expectedHeader, line);
            for (Particle& particle : simulationManager.particles) {
                std::getline(in, line);
                AssertParticleFromLine(line, particle);
            }
        }

        TEST_METHOD(InitSimulation_PresetSolarSystem)
        {
            SimulationManager simulationManager;
            simulationManager.particles.push_back(Particle());

            simulationManager.initSimulation(PresetType::PRESET_SOLAR_SYSTEM);

            Assert::AreEqual(static_cast<uint32_t>(9), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(static_cast<size_t>(9), simulationManager.particles.size());

            std::ifstream in("Presets/SolarSystem.csv");
            Assert::IsTrue(in.good(), L"File could not be opened");
            std::string line;
            std::getline(in, line);
            const std::string expectedHeader = "PositionX;PositionY;PositionZ;Mass;VelocityX;VelocityY;VelocityZ;Size;AccelerationX;AccelerationY;AccelerationZ;Force;ColorR;ColorG;ColorB;Movable";
            Assert::AreEqual(expectedHeader, line);
            for (Particle& particle : simulationManager.particles) {
                std::getline(in, line);
                AssertParticleFromLine(line, particle);
            }
        }
    };
}