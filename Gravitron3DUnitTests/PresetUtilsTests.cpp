#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/PresetUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(PresetUtilsTests)
    {
    public:
        TEST_METHOD(RandomFloat_Valid)
        {
            float min = -0.5f;
            float max = 0.5f;

            for (int i = 0; i < 10000; i++) {
                float returnedValue = PresetUtils::randomFloat(min, max);
                Assert::IsTrue(returnedValue >= min);
                Assert::IsTrue(returnedValue <= max);
            }
        }

        TEST_METHOD(RandomFloat_Invalid)
        {
            float min = 0.0f;
            float max = -0.0001f;

            Assert::ExpectException<std::runtime_error>([&]() {
                PresetUtils::randomFloat(min, max);
            });
        }

        TEST_METHOD(CalculatePositionsRandom_AllProperties)
        {
            int n = 10000;
            std::vector<Particle> particles;
            for (int i = 0; i < n; i++) {
                particles.push_back(Particle());
            }

            glm::vec3 particlePositionDefaultValue = glm::vec3(0.0f);
            int rangeMin = n / 2;
            int rangeMax = n;
            glm::vec3 minValue = glm::vec3(-10.0f, 0.0f, 10.0f);
            glm::vec3 maxValue = glm::vec3(-5.0f, 2.0f, 15.0f);

            PresetUtils::calculatePositionsRandom(particles, rangeMin, rangeMax, minValue, maxValue);
            for (int i = 0; i < rangeMin; i++) {
                Assert::AreEqual(particlePositionDefaultValue.x, particles[i].getPosition().x);
                Assert::AreEqual(particlePositionDefaultValue.y, particles[i].getPosition().y);
                Assert::AreEqual(particlePositionDefaultValue.z, particles[i].getPosition().z);
            }

            for (int i = rangeMin; i < rangeMax; i++) {
                Assert::IsTrue(particles[i].getPosition().x >= minValue.x);
                Assert::IsTrue(particles[i].getPosition().y >= minValue.y);
                Assert::IsTrue(particles[i].getPosition().z >= minValue.z);
                Assert::IsTrue(particles[i].getPosition().x <= maxValue.x);
                Assert::IsTrue(particles[i].getPosition().y <= maxValue.y);
                Assert::IsTrue(particles[i].getPosition().z <= maxValue.z);
            }
        }

        TEST_METHOD(CalculatePositionsSphere_ValidAllProperties)
        {
            int n = 10000;
            std::vector<Particle> particles;
            for (int i = 0; i < n; i++) {
                particles.push_back(Particle());
            }

            glm::vec3 particlePositionDefaultValue = glm::vec3(0.0f);
            int rangeMin = n / 2;
            int rangeMax = n;
            glm::vec3 centerPosition = glm::vec3(-10.0f, 0.0f, 10.0f);
            float radiusMin = 20.0f;
            float radiusMax = 250.0f;

            PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, centerPosition, radiusMin, radiusMax, false);
            for (int i = 0; i < rangeMin; i++) {
                Assert::AreEqual(particlePositionDefaultValue.x, particles[i].getPosition().x);
                Assert::AreEqual(particlePositionDefaultValue.y, particles[i].getPosition().y);
                Assert::AreEqual(particlePositionDefaultValue.z, particles[i].getPosition().z);
            }

            for (int i = rangeMin; i < rangeMax; i++) {
                float distance = glm::distance(particles[i].getPosition(), centerPosition);
                Assert::IsTrue(distance >= radiusMin);
                Assert::IsTrue(distance <= radiusMax);
            }
        }

        TEST_METHOD(CalculatePositionsSphere_InvalidRadiusMin)
        {
            int n = 10;
            std::vector<Particle> particles;
            for (int i = 0; i < n; i++) {
                particles.push_back(Particle());
            }

            glm::vec3 particlePositionDefaultValue = glm::vec3(0.0f);
            int rangeMin = n / 2;
            int rangeMax = n;
            glm::vec3 centerPosition = glm::vec3(-10.0f, 0.0f, 10.0f);
            float radiusMin = -0.00001f;
            float radiusMax = 250.0f;

            Assert::ExpectException<std::runtime_error>([&]() {
                PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, centerPosition, radiusMin, radiusMax, false);
            });
        }

        TEST_METHOD(CalculatePositionsSphere_InvalidRadiusMax)
        {
            int n = 10;
            std::vector<Particle> particles;
            for (int i = 0; i < n; i++) {
                particles.push_back(Particle());
            }

            glm::vec3 particlePositionDefaultValue = glm::vec3(0.0f);
            int rangeMin = n / 2;
            int rangeMax = n;
            glm::vec3 centerPosition = glm::vec3(-10.0f, 0.0f, 10.0f);
            float radiusMin = 20.0f;
            float radiusMax = 19.9999f;

            Assert::ExpectException<std::runtime_error>([&]() {
                PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, centerPosition, radiusMin, radiusMax, false);
            });
        }

        TEST_METHOD(CalculatePositionsSphere2D_ValidAllProperties)
        {
            int n = 10000;
            std::vector<Particle> particles;
            for (int i = 0; i < n; i++) {
                particles.push_back(Particle());
            }

            glm::vec3 particlePositionDefaultValue = glm::vec3(0.0f);
            int rangeMin = n / 2;
            int rangeMax = n;
            glm::vec3 centerPosition = glm::vec3(-10.0f, 0.0f, 10.0f);
            float radiusMin = 20.0f;
            float radiusMax = 250.0f;

            PresetUtils::calculatePositionsSphere(particles, rangeMin, rangeMax, centerPosition, radiusMin, radiusMax, true);
            for (int i = 0; i < rangeMin; i++) {
                Assert::AreEqual(particlePositionDefaultValue.x, particles[i].getPosition().x);
                Assert::AreEqual(particlePositionDefaultValue.y, particles[i].getPosition().y);
                Assert::AreEqual(particlePositionDefaultValue.z, particles[i].getPosition().z);
            }

            for (int i = rangeMin; i < rangeMax; i++) {
                float distance = glm::distance(particles[i].getPosition(), centerPosition);
                Assert::AreEqual(particles[i].getPosition().y, centerPosition.y);
                Assert::IsTrue(distance >= radiusMin);
                Assert::IsTrue(distance <= radiusMax);
            }
        }

        TEST_METHOD(CalculatePositionsGrid3D_AllProperties)
        {
            int n = 16;
            std::vector<Particle> particles;
            for (int i = 0; i < n; i++) {
                particles.push_back(Particle());
            }

            glm::vec3 particlePositionDefaultValue = glm::vec3(0.0f);
            int rangeMin = n / 2;
            int rangeMax = n;
            glm::vec3 minValue = glm::vec3(0.0f);
            glm::vec3 maxValue = glm::vec3(1.0f);

            PresetUtils::calculatePositionsGrid3D(particles, rangeMin, rangeMax, minValue, maxValue);
            for (int i = 0; i < rangeMin; i++) {
                Assert::AreEqual(particlePositionDefaultValue.x, particles[i].getPosition().x);
                Assert::AreEqual(particlePositionDefaultValue.y, particles[i].getPosition().y);
                Assert::AreEqual(particlePositionDefaultValue.z, particles[i].getPosition().z);
            }

            // The following logic only works with rangeMax-rangeMin=8, minValue=(0,0,0), maxValue = (1,1,1)
            for (int i = rangeMin; i < rangeMax; i++) {
                float expectedPositionX = int(floor(i / 4.0f)) % 2 == 0 ? 0.0f : 1.0f;
                float expectedPositionY = int(floor(i / 2.0f)) % 2 == 0 ? 0.0f : 1.0f;
                float expectedPositionZ = i % 2 == 0 ? 0.0f : 1.0f;

                Assert::AreEqual(expectedPositionX, particles[i].getPosition().x);
                Assert::AreEqual(expectedPositionY, particles[i].getPosition().y);
                Assert::AreEqual(expectedPositionZ, particles[i].getPosition().z);
            }
        }
    };
}