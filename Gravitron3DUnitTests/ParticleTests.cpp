#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/Particle.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(ParticleTests)
    {
    public:
        TEST_METHOD(Constructor_Default)
        {
            Particle particle = Particle();

            Assert::AreEqual(0.0f, particle.getPosition().x);
            Assert::AreEqual(0.0f, particle.getPosition().y);
            Assert::AreEqual(0.0f, particle.getPosition().z);
            Assert::AreEqual(1.0f, particle.getMass());
            Assert::AreEqual(0.0f, particle.getVelocity().x);
            Assert::AreEqual(0.0f, particle.getVelocity().y);
            Assert::AreEqual(0.0f, particle.getVelocity().z);
            Assert::AreEqual(1.0f, particle.getSize());
            Assert::AreEqual(0.0f, particle.getAcceleration().x);
            Assert::AreEqual(0.0f, particle.getAcceleration().y);
            Assert::AreEqual(0.0f, particle.getAcceleration().z);
            Assert::AreEqual(0.0f, particle.getForce());
            Assert::AreEqual(1.0f, particle.getColor().r);
            Assert::AreEqual(1.0f, particle.getColor().g);
            Assert::AreEqual(1.0f, particle.getColor().b);
            Assert::AreEqual(true, particle.getMovable());
        }

        TEST_METHOD(Constructor_Valid)
        {
            glm::vec3 expectedPosition = glm::vec3(-100.0f, 0.0f, 100.0f);
            float expectedMass = 15.5f;
            glm::vec3 expectedVelocity = glm::vec3(-10.0f, 0.0f, 10.0f);
            float expectedSize = 3.5f;
            glm::vec3 expectedAcceleration = glm::vec3(-1.0f, 0.0f, 1.0f);
            float expectedForce = 1.5f;
            glm::vec3 expectedColor = glm::vec3(0.0f, 0.35f, 1.0f);
            bool expectedMovable = false;

            Particle particle = Particle(
                glm::vec4(expectedPosition, expectedMass),
                glm::vec4(expectedVelocity, expectedSize),
                glm::vec4(expectedAcceleration, expectedForce),
                glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
            );

            Assert::AreEqual(expectedPosition.x, particle.getPosition().x);
            Assert::AreEqual(expectedPosition.y, particle.getPosition().y);
            Assert::AreEqual(expectedPosition.z, particle.getPosition().z);
            Assert::AreEqual(expectedMass, particle.getMass());
            Assert::AreEqual(expectedVelocity.x, particle.getVelocity().x);
            Assert::AreEqual(expectedVelocity.y, particle.getVelocity().y);
            Assert::AreEqual(expectedVelocity.z, particle.getVelocity().z);
            Assert::AreEqual(expectedSize, particle.getSize());
            Assert::AreEqual(expectedAcceleration.x, particle.getAcceleration().x);
            Assert::AreEqual(expectedAcceleration.y, particle.getAcceleration().y);
            Assert::AreEqual(expectedAcceleration.z, particle.getAcceleration().z);
            Assert::AreEqual(expectedForce, particle.getForce());
            Assert::AreEqual(expectedColor.r, particle.getColor().r);
            Assert::AreEqual(expectedColor.g, particle.getColor().g);
            Assert::AreEqual(expectedColor.b, particle.getColor().b);
            Assert::AreEqual(expectedMovable, particle.getMovable());
        }

        TEST_METHOD(Constructor_InvalidMass)
        {
            glm::vec3 expectedPosition = glm::vec3(-100.0f, 0.0f, 100.0f);
            glm::vec3 expectedVelocity = glm::vec3(-10.0f, 0.0f, 10.0f);
            float expectedSize = 3.5f;
            glm::vec3 expectedAcceleration = glm::vec3(-1.0f, 0.0f, 1.0f);
            float expectedForce = 1.5f;
            glm::vec3 expectedColor = glm::vec3(0.0f, 0.35f, 1.0f);
            bool expectedMovable = false;

            float expectedMass = 0.000009f;

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });

            expectedMass = -1.0f;

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });
        }

        TEST_METHOD(Constructor_InvalidSize)
        {
            glm::vec3 expectedPosition = glm::vec3(-100.0f, 0.0f, 100.0f);
            float expectedMass = 15.5f;
            glm::vec3 expectedVelocity = glm::vec3(-10.0f, 0.0f, 10.0f);
            glm::vec3 expectedAcceleration = glm::vec3(-1.0f, 0.0f, 1.0f);
            float expectedForce = 1.5f;
            glm::vec3 expectedColor = glm::vec3(0.0f, 0.35f, 1.0f);
            bool expectedMovable = false;

            float expectedSize = 0.09f;

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });

            expectedSize = -1.0f;

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });
        }

        TEST_METHOD(Constructor_InvalidForce)
        {
            glm::vec3 expectedPosition = glm::vec3(-100.0f, 0.0f, 100.0f);
            float expectedMass = 15.5f;
            glm::vec3 expectedVelocity = glm::vec3(-10.0f, 0.0f, 10.0f);
            float expectedSize = 1.0f;
            glm::vec3 expectedAcceleration = glm::vec3(-1.0f, 0.0f, 1.0f);
            glm::vec3 expectedColor = glm::vec3(0.0f, 0.35f, 1.0f);
            bool expectedMovable = false;

            float expectedForce = -0.000001f;

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });

            expectedForce = -100.0f;

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });
        }

        TEST_METHOD(Constructor_InvalidColor)
        {
            glm::vec3 expectedPosition = glm::vec3(-100.0f, 0.0f, 100.0f);
            float expectedMass = 15.5f;
            glm::vec3 expectedVelocity = glm::vec3(-10.0f, 0.0f, 10.0f);
            float expectedSize = 1.0f;
            glm::vec3 expectedAcceleration = glm::vec3(-1.0f, 0.0f, 1.0f);
            float expectedForce = 0.0f;
            bool expectedMovable = false;

            glm::vec3 expectedColor = glm::vec3(-0.00001f, 0.0f, 1.0f);

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });

            expectedColor = glm::vec3(1.0f, 0.0f, 1.00001f);

            Assert::ExpectException<std::runtime_error>([&]() {
                Particle(
                    glm::vec4(expectedPosition, expectedMass),
                    glm::vec4(expectedVelocity, expectedSize),
                    glm::vec4(expectedAcceleration, expectedForce),
                    glm::vec4(expectedColor, expectedMovable ? 1.0f : 0.0f)
                );
            });
        }
    };
}