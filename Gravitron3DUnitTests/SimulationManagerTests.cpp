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

        TEST_METHOD(UpdateSimulation_UpdatesCorrectly)
        {
            SimulationManager simulationManager;
            simulationManager.settings.setTheta(0.0f);
            simulationManager.settings.setEpsilon(0.0f);
            simulationManager.settings.setSimulationSpeed(1.0f);
            simulationManager.addParticle(
                glm::vec4(0.0f, 0.0f, 0.0f, 2.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            simulationManager.addParticle(
                glm::vec4(1.0f, 0.0f, 0.0f, 2.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f)
            );

            simulationManager.updateSimulation(1.0f);

            Assert::AreEqual(2.0f, simulationManager.particles[0].getPosition().x);
            Assert::AreEqual(0.0f, simulationManager.particles[0].getPosition().y);
            Assert::AreEqual(0.0f, simulationManager.particles[0].getPosition().z);
            Assert::AreEqual(2.0f, simulationManager.particles[0].getVelocity().x);
            Assert::AreEqual(0.0f, simulationManager.particles[0].getVelocity().y);
            Assert::AreEqual(0.0f, simulationManager.particles[0].getVelocity().z);
            Assert::AreEqual(2.0f, simulationManager.particles[0].getAcceleration().x);
            Assert::AreEqual(0.0f, simulationManager.particles[0].getAcceleration().y);
            Assert::AreEqual(0.0f, simulationManager.particles[0].getAcceleration().z);
            Assert::AreEqual(-1.0f, simulationManager.particles[1].getPosition().x);
            Assert::AreEqual(0.0f, simulationManager.particles[1].getPosition().y);
            Assert::AreEqual(0.0f, simulationManager.particles[1].getPosition().z);
            Assert::AreEqual(-2.0f, simulationManager.particles[1].getVelocity().x);
            Assert::AreEqual(0.0f, simulationManager.particles[1].getVelocity().y);
            Assert::AreEqual(0.0f, simulationManager.particles[1].getVelocity().z);
            Assert::AreEqual(-2.0f, simulationManager.particles[1].getAcceleration().x);
            Assert::AreEqual(0.0f, simulationManager.particles[1].getAcceleration().y);
            Assert::AreEqual(0.0f, simulationManager.particles[1].getAcceleration().z);
        }

        TEST_METHOD(AddParticle_AllParticlesDataCorrect)
        {
            SimulationManager simulationManager;
            simulationManager.initSimulation(PresetType::PRESET_SOLAR_SYSTEM);
            std::vector<Particle> originalParticles = simulationManager.particles;

            glm::vec4 positionMass = glm::vec4(-500.0f, 0.0f, 500.0f, 115.5f);
            glm::vec4 velocitySize = glm::vec4(-50.0f, 0.0f, 50.0f, 12.5f);
            glm::vec4 accelerationForce = glm::vec4(-5.0f, 0.0f, 5.0f, 1.5f);
            glm::vec4 colorMovable = glm::vec4(0.0f, 0.5f, 1.0f, 1.0f);
            simulationManager.addParticle(
                positionMass,
                velocitySize,
                accelerationForce,
                colorMovable
            );

            Assert::AreEqual(originalParticles.size() + 1, simulationManager.particles.size());
            Assert::AreEqual(static_cast<uint32_t>(originalParticles.size() + 1), simulationManager.settings.getNumberOfParticles());

            int i = 0;
            while (i < originalParticles.size()) {
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getPosition(), originalParticles[i].getMass()),
                    glm::vec4(simulationManager.particles[i].getPosition(), simulationManager.particles[i].getMass()));
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getVelocity(), originalParticles[i].getSize()),
                    glm::vec4(simulationManager.particles[i].getVelocity(), simulationManager.particles[i].getSize()));
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getAcceleration(), originalParticles[i].getForce()),
                    glm::vec4(simulationManager.particles[i].getAcceleration(), simulationManager.particles[i].getForce()));
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getColor(), originalParticles[i].getMovable() ? 1.0f : 0.0f),
                    glm::vec4(simulationManager.particles[i].getColor(), simulationManager.particles[i].getMovable() ? 1.0f : 0.0f));
                i++;
            }

            AssertVec4Equal(
                positionMass,
                glm::vec4(simulationManager.particles[i].getPosition(), simulationManager.particles[i].getMass()));
            AssertVec4Equal(
                velocitySize,
                glm::vec4(simulationManager.particles[i].getVelocity(), simulationManager.particles[i].getSize()));
            AssertVec4Equal(
                accelerationForce,
                glm::vec4(simulationManager.particles[i].getAcceleration(), simulationManager.particles[i].getForce()));
            AssertVec4Equal(
                colorMovable,
                glm::vec4(simulationManager.particles[i].getColor(), simulationManager.particles[i].getMovable() ? 1.0f : 0.0f));
        }

        TEST_METHOD(AddGroup_AllParticlesDataCorrect)
        {
            SimulationManager simulationManager;
            simulationManager.initSimulation(PresetType::PRESET_SOLAR_SYSTEM);
            std::vector<Particle> originalParticles = simulationManager.particles;

            SpawnRegion spawnRegion = {
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                glm::vec3(-5.0f, 0.2f, 6.0f),
                25.0f,
                250.0f
            };
            VelocityProperties velocityProperties = {
                VelocityType::VELOCITY_RANDOM,
                1.0f,
                glm::vec3(-10.0f, 0.0f, 10.0f),
                glm::vec3(  5.0f, 0.0f, 25.0f),
                glm::vec3(1.5f, 0.5f, -1.5f)
            };
            MassProperties massProperties = {
                MassType::MASS_RANDOM,
                1.0f,
                0.5f,
                15.0f
            };
            SizeProperties sizeProperties = {
                SizeType::SIZE_RANDOM,
                1.0f,
                0.5f,
                5.0f
            };
            ParticleGroupConfig config = {
                500,
                PositionType::POSITION_SPHERE,
                spawnRegion,
                velocityProperties,
                true,
                151.0f,
                massProperties,
                sizeProperties
            };

            simulationManager.addGroup(config);

            Assert::AreEqual(originalParticles.size() + config.numberOfParticlesToAdd, simulationManager.particles.size());
            Assert::AreEqual(static_cast<uint32_t>(originalParticles.size() + config.numberOfParticlesToAdd), simulationManager.settings.getNumberOfParticles());

            int i = 0;
            while (i < originalParticles.size()) {
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getPosition(), originalParticles[i].getMass()),
                    glm::vec4(simulationManager.particles[i].getPosition(), simulationManager.particles[i].getMass()));
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getVelocity(), originalParticles[i].getSize()),
                    glm::vec4(simulationManager.particles[i].getVelocity(), simulationManager.particles[i].getSize()));
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getAcceleration(), originalParticles[i].getForce()),
                    glm::vec4(simulationManager.particles[i].getAcceleration(), simulationManager.particles[i].getForce()));
                AssertVec4Equal(
                    glm::vec4(originalParticles[i].getColor(), originalParticles[i].getMovable() ? 1.0f : 0.0f),
                    glm::vec4(simulationManager.particles[i].getColor(), simulationManager.particles[i].getMovable() ? 1.0f : 0.0f));
                i++;
            }

            AssertVec4Equal(
                glm::vec4(config.region.sphereCenter, 1.0f),
                glm::vec4(simulationManager.particles[i].getPosition(), simulationManager.particles[i].getMass()));
            AssertVec4Equal(
                glm::vec4(config.velocity.overallVelocity, 1.0f),
                glm::vec4(simulationManager.particles[i].getVelocity(), simulationManager.particles[i].getSize()));
            AssertVec4Equal(
                glm::vec4(0.0f),
                glm::vec4(simulationManager.particles[i].getAcceleration(), simulationManager.particles[i].getForce()));
            AssertVec4Equal(
                glm::vec4(1.0f),
                glm::vec4(simulationManager.particles[i].getColor(), simulationManager.particles[i].getMovable() ? 1.0f : 0.0f));
            i++;
        
            while (i < simulationManager.particles.size()) {
                float distance = glm::distance(simulationManager.particles[i].getPosition(), config.region.sphereCenter);
                Assert::IsTrue(distance >= config.region.sphereRadiusMin);
                Assert::IsTrue(distance <= config.region.sphereRadiusMax);

                Assert::IsTrue(simulationManager.particles[i].getMass() >= config.mass.randomMin);
                Assert::IsTrue(simulationManager.particles[i].getMass() <= config.mass.randomMax);

                Assert::IsTrue(simulationManager.particles[i].getVelocity().x >= config.velocity.randomMin.x + config.velocity.overallVelocity.x);
                Assert::IsTrue(simulationManager.particles[i].getVelocity().x <= config.velocity.randomMax.x + config.velocity.overallVelocity.x);
                Assert::IsTrue(simulationManager.particles[i].getVelocity().y >= config.velocity.randomMin.y + config.velocity.overallVelocity.y);
                Assert::IsTrue(simulationManager.particles[i].getVelocity().y <= config.velocity.randomMax.y + config.velocity.overallVelocity.y);
                Assert::IsTrue(simulationManager.particles[i].getVelocity().z >= config.velocity.randomMin.z + config.velocity.overallVelocity.z);
                Assert::IsTrue(simulationManager.particles[i].getVelocity().z <= config.velocity.randomMax.z + config.velocity.overallVelocity.z);

                Assert::IsTrue(simulationManager.particles[i].getSize() >= config.size.randomMin);
                Assert::IsTrue(simulationManager.particles[i].getSize() <= config.size.randomMax);

                AssertVec4Equal(glm::vec4(0.0f), glm::vec4(simulationManager.particles[i].getAcceleration(), simulationManager.particles[i].getForce()));
                AssertVec4Equal(glm::vec4(1.0f), glm::vec4(simulationManager.particles[i].getColor(), simulationManager.particles[i].getMovable() ? 1.0f : 0.0f));

                i++;
            }
        }

        TEST_METHOD(LoadSettings_ReadsCorrectContent)
        {
            SimulationManager simulationManager;
            simulationManager.loadSettings("Settings/customTest.stg");

            Assert::AreEqual(6, simulationManager.settings.getVersion());
            Assert::AreEqual(static_cast<uint32_t>(0), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(0.35f, simulationManager.settings.getSimulationSpeed());
            Assert::AreEqual(static_cast<uint32_t>(8), simulationManager.settings.getNumberOfThreads());
            Assert::AreEqual(0.3f, simulationManager.settings.getTheta());
            Assert::AreEqual(5.0f, simulationManager.settings.getEpsilon());
            Assert::AreEqual(0.0f, simulationManager.settings.getLightPos().x);
            Assert::AreEqual(-1.0f, simulationManager.settings.getLightPos().y);
            Assert::AreEqual(0.0f, simulationManager.settings.getLightPos().z);
            Assert::AreEqual(0.5f, simulationManager.settings.getLightPos().w);
            Assert::AreEqual(0.1f, simulationManager.settings.getLightConstantAttenuation());
            Assert::AreEqual(0.2f, simulationManager.settings.getLightLinearAttenuation());
            Assert::AreEqual(0.3f, simulationManager.settings.getLightQuadraticAttenuation());
            Assert::AreEqual(0.01f, simulationManager.settings.getScaleFactor());
            Assert::AreEqual(true, simulationManager.settings.getIsForceColor());
            Assert::AreEqual(10.0f, simulationManager.settings.getMinForceColor());
            Assert::AreEqual(90.0f, simulationManager.settings.getMaxForceColor());
            Assert::AreEqual(0.1f, simulationManager.settings.getBackgroundColor().r);
            Assert::AreEqual(0.1f, simulationManager.settings.getBackgroundColor().g);
            Assert::AreEqual(0.1f, simulationManager.settings.getBackgroundColor().b);
        }

        TEST_METHOD(DefaultSettings_SetBackCorrectly)
        {
            SimulationManager simulationManager;
            int n = 10;
            for (int i = 0; i < n; i++)
                simulationManager.addParticle(glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f));
            float defaultTheta = simulationManager.settings.getTheta();
            float defaultEpsilon = simulationManager.settings.getEpsilon();
            bool defaultIsForceColor = simulationManager.settings.getIsForceColor();

            float newTheta = 0.9f;
            float newEpsilon = 1.5f;
            simulationManager.settings.setTheta(newTheta);
            simulationManager.settings.setEpsilon(newEpsilon);
            simulationManager.settings.setIsForceColor(!defaultIsForceColor);

            Assert::AreEqual(static_cast<uint32_t>(n), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(newTheta, simulationManager.settings.getTheta());
            Assert::AreEqual(newEpsilon, simulationManager.settings.getEpsilon());
            Assert::AreEqual(!defaultIsForceColor, simulationManager.settings.getIsForceColor());

            simulationManager.defaultSettings();

            Assert::AreEqual(static_cast<uint32_t>(n), simulationManager.settings.getNumberOfParticles());
            Assert::AreEqual(defaultTheta, simulationManager.settings.getTheta());
            Assert::AreEqual(defaultEpsilon, simulationManager.settings.getEpsilon());
            Assert::AreEqual(defaultIsForceColor, simulationManager.settings.getIsForceColor());
        }

        TEST_METHOD(LoadParticles_ReadsCorrectContent)
        {
            SimulationManager simulationManager;
            simulationManager.loadParticles("Particles/SolarSystem.csv");
            std::vector<Particle> expected;
            expected.push_back(Particle(
                glm::vec4(10000.00f, -227.70f, -130.4f, 102.41f),
                glm::vec4(0.09f, -0.28f, 13.46f, 12.30f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.25f, 0.47f, 0.85f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(3628.00f, -26.8f, 5441.00f, 86.81f),
                glm::vec4(-14.07f, 0.21f, 8.52f, 12.70f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.60f, 0.80f, 0.85f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(3181.00f, -118.80f, -447.50f, 568.34f),
                glm::vec4(1.99f, -0.49f, 23.52f, 29.10f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.85f, 0.72f, 0.53f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(152.00f, -10.50f, 1700.00f, 1898.19f),
                glm::vec4(-32.38f, 0.71f, 4.40f, 34.95f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.80f, 0.67f, 0.50f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-453.90f, 17.90f, 321.80f, 0.64f),
                glm::vec4(-32.43f, -0.11f, -43.35f, 1.70f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.79f, 0.36f, 0.21f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-335.00f, 0.10f, -3.30f, 5.97f),
                glm::vec4(-0.84f, 0.00f, -73.51f, 3.20f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.27f, 0.50f, 0.75f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-242.20f, 14.00f, 2.50f, 4.87f),
                glm::vec4(-1.98f, -1.07f, -86.57f, 3.05f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.90f, 0.76f, 0.52f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-122.90f, 13.20f, 23.20f, 0.33f),
                glm::vec4(-48.98f, -4.68f, -112.20f, 1.20f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(0.42f, 0.38f, 0.35f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-1.70f, 0.06f, -1.70f, 1988410.00f),
                glm::vec4(0.03f, 0.00f, -0.01f, 84.75f),
                glm::vec4(0.00f, 0.00f, 0.00f, 0.00f),
                glm::vec4(1.00f, 0.85f, 0.00f, 1.00f)
            ));

            size_t n = expected.size();
            Assert::AreEqual(n, simulationManager.particles.size());
            Assert::AreEqual(static_cast<uint32_t>(n), simulationManager.settings.getNumberOfParticles());
            for (int i = 0; i < n; i++) {
                AssertVec4Equal(glm::vec4(expected[i].getPosition(), expected[i].getMass()), glm::vec4(simulationManager.particles[i].getPosition(), simulationManager.particles[i].getMass()));
                AssertVec4Equal(glm::vec4(expected[i].getVelocity(), expected[i].getSize()), glm::vec4(simulationManager.particles[i].getVelocity(), simulationManager.particles[i].getSize()));
                AssertVec4Equal(glm::vec4(expected[i].getAcceleration(), expected[i].getForce()), glm::vec4(simulationManager.particles[i].getAcceleration(), simulationManager.particles[i].getForce()));
                AssertVec4Equal(glm::vec4(expected[i].getColor(), expected[i].getMovable() ? 1.0f : 0.0f), glm::vec4(simulationManager.particles[i].getColor(), simulationManager.particles[i].getMovable() ? 1.0f : 0.0f));
            }
        }
    };
}