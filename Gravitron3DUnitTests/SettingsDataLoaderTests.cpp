#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Persistence/SettingsDataLoader.h"

#include <fstream>
#include <string>
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(SettingsDataLoaderTests)
    {
    public:
        void AssertBoolField(const std::string& line, const std::string& key, bool expected) {
            Assert::IsTrue(line.rfind(key + "=", 0) == 0);
            std::string value = line.substr(key.size() + 1);
            bool actual = (value == "1");
            Assert::AreEqual(expected, actual);
        }

        void AssertIntField(const std::string& line, const std::string& key, int expected) {
            Assert::IsTrue(line.rfind(key + "=", 0) == 0);
            int actual = std::stoi(line.substr(key.size() + 1));
            Assert::AreEqual(expected, actual);
        }

        void AssertFloatField(const std::string& line, const std::string& key, float expected, float epsilon = 0.0001f) {
            Assert::IsTrue(line.rfind(key + "=", 0) == 0);
            float actual = std::stof(line.substr(key.size() + 1));
            Assert::AreEqual(expected, actual, epsilon);
        }

        void AssertVec3Field(const std::string& line, const std::string& key, const float expected[3], float epsilon = 0.0001f) {
            Assert::IsTrue(line.rfind(key + "=", 0) == 0);
            std::string values = line.substr(key.size() + 1);
            std::stringstream ss(values);
            std::string component;
            int i = 0;
            while (std::getline(ss, component, ',') && i < 3) {
                float actual = std::stof(component);
                Assert::AreEqual(expected[i], actual, epsilon);
                i++;
            }
            Assert::AreEqual(3, i);
        }

        void AssertVec4Field(const std::string& line, const std::string& key, const float expected[4], float epsilon = 0.0001f) {
            Assert::IsTrue(line.rfind(key + "=", 0) == 0);
            std::string values = line.substr(key.size() + 1);
            std::stringstream ss(values);
            std::string component;
            int i = 0;
            while (std::getline(ss, component, ',') && i < 4) {
                float actual = std::stof(component);
                Assert::AreEqual(expected[i], actual, epsilon);
                i++;
            }
            Assert::AreEqual(4, i);
        }

        TEST_METHOD(SaveToFile_WritesCorrectContent)
        {
            // Arrange
            SettingsDataLoader loader;
            Settings settings;
            int version = settings.getVersion();
            uint32_t numberOfParticles = 15;
            settings.setNumberOfParticles(numberOfParticles);
            float simulationSpeed = 0.3f;
            settings.setSimulationSpeed(simulationSpeed);
            uint32_t numberOfThreads = 8;
            settings.setNumberOfThreads(numberOfThreads);
            float theta = 0.25f;
            settings.setTheta(theta);
            float epsilon = 1.3f;
            settings.setEpsilon(epsilon);
            glm::vec4 lightPos = glm::vec4(0.2f, -0.3f, 0.7f, 1.0f);
            settings.setLightPos(lightPos);
            float lightConstantAttenuation = 0.1f;
            settings.setLightConstantAttenuation(lightConstantAttenuation);
            float lightLinearAttenuation = 0.4f;
            settings.setLightLinearAttenuation(lightLinearAttenuation);
            float lightQuadraticAttenuation = 0.8f;
            settings.setLightQuadraticAttenuation(lightQuadraticAttenuation);
            float scaleFactor = 0.015f;
            settings.setScaleFactor(scaleFactor);
            bool isForceColor = true;
            settings.setIsForceColor(isForceColor);
            float minForceColor = 0.975f;
            settings.setMinForceColor(minForceColor);
            float maxForceColor = 85.5f;
            settings.setMaxForceColor(85.5f);
            glm::vec3 backgroundColor = glm::vec3(0.5f, 0.3f, 0.7f);
            settings.setBackgroundColor(backgroundColor);

            std::string filename = "test_output.txt";

            // Act
            SettingsDataLoader::saveToFile(filename, settings);

            // Assert
            std::ifstream in(filename);
            Assert::IsTrue(in.good(), L"File could not be opened");

            std::string line;

            std::getline(in, line);
            AssertIntField(line, "version", version);

            std::getline(in, line);
            AssertIntField(line, "numberOfParticles", numberOfParticles);

            std::getline(in, line);
            AssertFloatField(line, "simulationSpeed", simulationSpeed);

            std::getline(in, line);
            AssertIntField(line, "numberOfThreads", numberOfThreads);

            std::getline(in, line);
            AssertFloatField(line, "theta", theta);

            std::getline(in, line);
            AssertFloatField(line, "epsilon", epsilon);

            std::getline(in, line);
            float expectedLightPos[4] = { lightPos.x, lightPos.y, lightPos.z, lightPos.w };
            AssertVec4Field(line, "lightPos", expectedLightPos);

            std::getline(in, line);
            AssertFloatField(line, "lightConstantAttenuation", lightConstantAttenuation);

            std::getline(in, line);
            AssertFloatField(line, "lightLinearAttenuation", lightLinearAttenuation);

            std::getline(in, line);
            AssertFloatField(line, "lightQuadraticAttenuation", lightQuadraticAttenuation);

            std::getline(in, line);
            AssertFloatField(line, "scaleFactor", scaleFactor);

            std::getline(in, line);
            AssertBoolField(line, "isForceColor", isForceColor);

            std::getline(in, line);
            AssertFloatField(line, "minForceColor", minForceColor);

            std::getline(in, line);
            AssertFloatField(line, "maxForceColor", maxForceColor);

            std::getline(in, line);
            float expectedBackgroundColor[4] = { backgroundColor.x, backgroundColor.y, backgroundColor.z };
            AssertVec3Field(line, "backgroundColor", expectedBackgroundColor);

            in.close();

            // Cleanup
            std::filesystem::remove(filename);
        }
    };
}