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

        TEST_METHOD(SetterMass_Valid)
        {
            Particle particle = Particle();

            float expectedMass = 0.00001f;
            particle.setMass(expectedMass);
            Assert::AreEqual(expectedMass, particle.getMass());

            expectedMass = 1000.0f;
            particle.setMass(expectedMass);
            Assert::AreEqual(expectedMass, particle.getMass());
        }

        TEST_METHOD(SetterMass_Invalid)
        {
            Particle particle = Particle();

            float expectedMass = 0.000009f;
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setMass(expectedMass);
            });

            expectedMass = -1.0f;
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setMass(expectedMass);
            });
        }

        TEST_METHOD(SetterPosition_Valid)
        {
            Particle particle = Particle();

            glm::vec3 expectedPosition = glm::vec3(-100.0f, 0.0f, 100.0f);
            particle.setPosition(expectedPosition);
            Assert::AreEqual(expectedPosition.x, particle.getPosition().x);
            Assert::AreEqual(expectedPosition.y, particle.getPosition().y);
            Assert::AreEqual(expectedPosition.z, particle.getPosition().z);
        }

        TEST_METHOD(SetterSize_Valid)
        {
            Particle particle = Particle();

            float expectedSize = 0.1f;
            particle.setSize(expectedSize);
            Assert::AreEqual(expectedSize, particle.getSize());

            expectedSize = 1000.0f;
            particle.setSize(expectedSize);
            Assert::AreEqual(expectedSize, particle.getSize());
        }

        TEST_METHOD(SetterSize_Invalid)
        {
            Particle particle = Particle();

            float expectedSize = 0.09f;
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setSize(expectedSize);
            });

            expectedSize = -1.0f;
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setSize(expectedSize);
            });
        }

        TEST_METHOD(SetterVelocity_Valid)
        {
            Particle particle = Particle();

            glm::vec3 expectedVelocity = glm::vec3(-100.0f, 0.0f, 100.0f);
            particle.setVelocity(expectedVelocity);
            Assert::AreEqual(expectedVelocity.x, particle.getVelocity().x);
            Assert::AreEqual(expectedVelocity.y, particle.getVelocity().y);
            Assert::AreEqual(expectedVelocity.z, particle.getVelocity().z);
        }

        TEST_METHOD(SetterForce_Valid)
        {
            Particle particle = Particle();

            float expectedForce = 0.0f;
            particle.setForce(expectedForce);
            Assert::AreEqual(expectedForce, particle.getForce());

            expectedForce = 1000.0f;
            particle.setForce(expectedForce);
            Assert::AreEqual(expectedForce, particle.getForce());
        }

        TEST_METHOD(SetterForce_Invalid)
        {
            Particle particle = Particle();

            float expectedForce = -0.00001f;
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setForce(expectedForce);
            });

            expectedForce = -1.0f;
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setForce(expectedForce);
            });
        }

        TEST_METHOD(SetterAcceleration_Valid)
        {
            Particle particle = Particle();

            glm::vec3 expectedAcceleration = glm::vec3(-100.0f, 0.0f, 100.0f);
            particle.setAcceleration(expectedAcceleration);
            Assert::AreEqual(expectedAcceleration.x, particle.getAcceleration().x);
            Assert::AreEqual(expectedAcceleration.y, particle.getAcceleration().y);
            Assert::AreEqual(expectedAcceleration.z, particle.getAcceleration().z);
        }

        TEST_METHOD(SetterMovable_Valid)
        {
            Particle particle = Particle();

            bool expectedMovable = false;
            particle.setMovable(expectedMovable);
            Assert::AreEqual(expectedMovable, particle.getMovable());

            expectedMovable = true;
            particle.setMovable(expectedMovable);
            Assert::AreEqual(expectedMovable, particle.getMovable());
        }

        TEST_METHOD(SetterColor_Valid)
        {
            Particle particle = Particle();

            glm::vec3 expectedColor = glm::vec3(0.0f, 0.5f, 1.0f);
            particle.setColor(expectedColor);
            Assert::AreEqual(expectedColor.x, particle.getColor().r);
            Assert::AreEqual(expectedColor.y, particle.getColor().g);
            Assert::AreEqual(expectedColor.z, particle.getColor().b);
        }

        TEST_METHOD(SetterColor_InvalidSmallerThanZero)
        {
            Particle particle = Particle();

            glm::vec3 expectedColor = glm::vec3(0.0f, -0.00001f, 1.0f);
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setColor(expectedColor);
            });
        }

        TEST_METHOD(SetterColor_InvalidGreaterThanOne)
        {
            Particle particle = Particle();

            glm::vec3 expectedColor = glm::vec3(0.0f, 1.00001f, 1.0f);
            Assert::ExpectException<std::runtime_error>([&]() {
                particle.setColor(expectedColor);
            });
        }
    };
}