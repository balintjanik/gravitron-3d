#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Persistence/ParticleDataLoader.h"

#include <fstream>
#include <string>
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(ParticleDataLoaderTests)
    {
    public:
        inline void AssertVec4Equal(const glm::vec4& expected, const glm::vec4& actual, float epsilon = 0.0001f) {
            Assert::AreEqual(expected.x, actual.x, epsilon);
            Assert::AreEqual(expected.y, actual.y, epsilon);
            Assert::AreEqual(expected.z, actual.z, epsilon);
            Assert::AreEqual(expected.w, actual.w, epsilon);
        }

        inline void AssertParticleFromLine(const std::string& line, const Particle& expected, float epsilon = 0.0001f) {
            std::stringstream ss(line);
            std::string value;
            float components[16];
            int i = 0;

            while (std::getline(ss, value, ';') && i < 16) {
                components[i++] = std::stof(value);
            }

            Assert::AreEqual(16, i);

            glm::vec4 positionMass(components[0],  components[1],  components[2],  components[3]);
            glm::vec4 velocitySize(components[4],  components[5],  components[6],  components[7]);
            glm::vec4 accelerationForce(components[8],  components[9],  components[10], components[11]);
            glm::vec4 colorMovable(components[12], components[13], components[14], components[15]);

            AssertVec4Equal(glm::vec4(expected.getPosition(), expected.getMass()), positionMass, epsilon);
            AssertVec4Equal(glm::vec4(expected.getVelocity(), expected.getSize()), velocitySize, epsilon);
            AssertVec4Equal(glm::vec4(expected.getAcceleration(), expected.getForce()), accelerationForce, epsilon);
            AssertVec4Equal(glm::vec4(expected.getColor(), expected.getMovable() ? 1.0f : 0.0f), colorMovable, epsilon);
        }

        TEST_METHOD(SaveToFile_WritesCorrectContent)
        {
            // Setup
            ParticleDataLoader loader;
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(-50.0f, 0.0f, 50.0f, 12.50f),
                glm::vec4( -3.0f, 0.0f,  3.0f,  8.25f),
                glm::vec4( -1.0f, 0.0f,  1.0f, 50.00f),
                glm::vec4(  0.0f, 0.5f,  1.0f,  1.00f)
            ));
            particles.push_back(Particle(
                glm::vec4(5000.00f, 0.00f, -5000.00f, 1200.50f),
                glm::vec4(  30.00f, 0.00f,   -30.00f,   80.25f),
                glm::vec4( -10.00f, 0.00f,    10.00f,  250.00f),
                glm::vec4(   0.15f, 0.55f,     0.98f,    0.00f)
            ));

            std::string filename = "test_output.txt";

            // Save
            ParticleDataLoader::saveToFile(filename, particles);

            // Assert
            std::ifstream in(filename);
            Assert::IsTrue(in.good(), L"File could not be opened");

            std::string line;

            // Header
            std::getline(in, line);
            const std::string expectedHeader = "PositionX;PositionY;PositionZ;Mass;VelocityX;VelocityY;VelocityZ;Size;AccelerationX;AccelerationY;AccelerationZ;Force;ColorR;ColorG;ColorB;Movable";
            Assert::AreEqual(expectedHeader, line);

            // Particles
            for (Particle& particle : particles) {
                std::getline(in, line);
                AssertParticleFromLine(line, particle);
            }

            in.close();

            // Cleanup
            std::filesystem::remove(filename);
        }

        TEST_METHOD(LoadFromFile_ReadsCorrectContent)
        {
            std::vector<Particle> actual = ParticleDataLoader::loadFromFile("Particles/SolarSystem.csv");
            std::vector<Particle> expected;
            expected.push_back(Particle(
                glm::vec4(10000.00f, -227.70f, -130.4f, 102.41f),
                glm::vec4(    0.09f,   -0.28f,   13.46f, 12.30f),
                glm::vec4(    0.00f,    0.00f,   0.00f,   0.00f),
                glm::vec4(    0.25f,    0.47f,   0.85f,   1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(3628.00f, -26.8f, 5441.00f, 86.81f),
                glm::vec4( -14.07f,   0.21f,   8.52f, 12.70f),
                glm::vec4(   0.00f,   0.00f,   0.00f,  0.00f),
                glm::vec4(   0.60f,   0.80f,   0.85f,  1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(3181.00f, -118.80f, -447.50f, 568.34f),
                glm::vec4(   1.99f,   -0.49f,   23.52f,  29.10f),
                glm::vec4(   0.00f,    0.00f,    0.00f,   0.00f),
                glm::vec4(   0.85f,    0.72f,    0.53f,   1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(152.00f, -10.50f, 1700.00f, 1898.19f),
                glm::vec4(-32.38f,   0.71f,    4.40f,   34.95f),
                glm::vec4(  0.00f,   0.00f,    0.00f,    0.00f),
                glm::vec4(  0.80f,   0.67f,    0.50f,    1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-453.90f, 17.90f, 321.80f, 0.64f),
                glm::vec4( -32.43f, -0.11f, -43.35f, 1.70f),
                glm::vec4(   0.00f,  0.00f,   0.00f, 0.00f),
                glm::vec4(   0.79f,  0.36f,   0.21f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-335.00f, 0.10f,  -3.30f, 5.97f),
                glm::vec4(  -0.84f, 0.00f, -73.51f, 3.20f),
                glm::vec4(   0.00f, 0.00f,   0.00f, 0.00f),
                glm::vec4(   0.27f, 0.50f,   0.75f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-242.20f, 14.00f,   2.50f, 4.87f),
                glm::vec4(  -1.98f, -1.07f, -86.57f, 3.05f),
                glm::vec4(   0.00f,  0.00f,   0.00f, 0.00f),
                glm::vec4(   0.90f,  0.76f,   0.52f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-122.90f, 13.20f,   23.20f, 0.33f),
                glm::vec4( -48.98f, -4.68f, -112.20f, 1.20f),
                glm::vec4(   0.00f,  0.00f,    0.00f, 0.00f),
                glm::vec4(   0.42f,  0.38f,    0.35f, 1.00f)
            ));
            expected.push_back(Particle(
                glm::vec4(-1.70f, 0.06f, -1.70f, 1988410.00f),
                glm::vec4( 0.03f, 0.00f, -0.01f,      84.75f),
                glm::vec4( 0.00f, 0.00f,  0.00f,       0.00f),
                glm::vec4( 1.00f, 0.85f,  0.00f,       1.00f)
            ));

            size_t n = expected.size();
            Assert::AreEqual(n, actual.size());
            for (int i = 0; i < n; i++) {
                AssertVec4Equal(glm::vec4(expected[i].getPosition(), expected[i].getMass()), glm::vec4(actual[i].getPosition(), actual[i].getMass()));
                AssertVec4Equal(glm::vec4(expected[i].getVelocity(), expected[i].getSize()), glm::vec4(actual[i].getVelocity(), actual[i].getSize()));
                AssertVec4Equal(glm::vec4(expected[i].getAcceleration(), expected[i].getForce()), glm::vec4(actual[i].getAcceleration(), actual[i].getForce()));
                AssertVec4Equal(glm::vec4(expected[i].getColor(), expected[i].getMovable() ? 1.0f : 0.0f), glm::vec4(actual[i].getColor(), actual[i].getMovable() ? 1.0f : 0.0f));
            }
        }

        TEST_METHOD(LoadFromFile_InvalidTestMass)
        {
            Assert::ExpectException<std::runtime_error>([&]() {
                ParticleDataLoader::loadFromFile("Particles/invalidTestMass.csv");
            });
        }

        TEST_METHOD(LoadFromFile_InvalidTestSize)
        {
            Assert::ExpectException<std::runtime_error>([&]() {
                ParticleDataLoader::loadFromFile("Particles/invalidTestSize.csv");
            });
        }

        TEST_METHOD(LoadFromFile_InvalidTestForce)
        {
            Assert::ExpectException<std::runtime_error>([&]() {
                ParticleDataLoader::loadFromFile("Particles/invalidTestForce.csv");
            });
        }

        TEST_METHOD(LoadFromFile_InvalidTestColorLower)
        {
            Assert::ExpectException<std::runtime_error>([&]() {
                ParticleDataLoader::loadFromFile("Particles/invalidTestColorLower.csv");
            });
        }

        TEST_METHOD(LoadFromFile_InvalidTestColorHigher)
        {
            Assert::ExpectException<std::runtime_error>([&]() {
                ParticleDataLoader::loadFromFile("Particles/invalidTestColorHigher.csv");
            });
        }
    };
}