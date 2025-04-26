#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/Settings.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(SettingsTests)
    {
    public:
        const float tolerance = 0.0001f;
        TEST_METHOD(SetNumberOfParticles_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            uint32_t withinRangeValue = 5000;
            settings.setNumberOfParticles(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getNumberOfParticles());

            // Below minimum
            int belowMinimumValue = settings.getMinNumberOfParticles() - 1;
            settings.setNumberOfParticles(belowMinimumValue);
            Assert::AreEqual(settings.getMinNumberOfParticles(), settings.getNumberOfParticles());

            // Above maximum
            uint32_t aboveMaximumValue = settings.getMaxNumberOfParticles() + 1;
            settings.setNumberOfParticles(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxNumberOfParticles(), settings.getNumberOfParticles());
        }

        TEST_METHOD(SetSimulationSpeed_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            float withinRangeValue = 0.8f;
            settings.setSimulationSpeed(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getSimulationSpeed(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinSimulationSpeed() - 0.01f;
            settings.setSimulationSpeed(belowMinimumValue);
            Assert::AreEqual(settings.getMinSimulationSpeed(), settings.getSimulationSpeed(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxSimulationSpeed() + 0.01f;
            settings.setSimulationSpeed(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxSimulationSpeed(), settings.getSimulationSpeed(), tolerance);
        }

        TEST_METHOD(SetNumberOfThreads_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            uint32_t withinRangeValue = 8;
            settings.setNumberOfThreads(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getNumberOfThreads());

            // Negative below minimum
            int belowMinimumValueInt = -1;
            settings.setNumberOfThreads(belowMinimumValueInt);
            Assert::AreEqual(static_cast<uint32_t>(1), settings.getNumberOfThreads());

            // Positive below minimum
            uint32_t belowMinimumValue = 0;
            settings.setNumberOfThreads(belowMinimumValue);
            Assert::AreEqual(static_cast<uint32_t>(1), settings.getNumberOfThreads());
        }

        TEST_METHOD(SetTheta_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            float withinRangeValue = 0.5f;
            settings.setTheta(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getTheta(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinTheta() - 0.01f;
            settings.setTheta(belowMinimumValue);
            Assert::AreEqual(settings.getMinTheta(), settings.getTheta(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxTheta() + 0.01f;
            settings.setTheta(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxTheta(), settings.getTheta(), tolerance);
        }

        TEST_METHOD(SetEpsilon_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            float withinRangeValue = 3.2f;
            settings.setEpsilon(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getEpsilon(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinEpsilon() - 0.01f;
            settings.setEpsilon(belowMinimumValue);
            Assert::AreEqual(settings.getMinEpsilon(), settings.getEpsilon(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxEpsilon() + 0.01f;
            settings.setEpsilon(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxEpsilon(), settings.getEpsilon(), tolerance);
        }

        TEST_METHOD(SetLightPos)
        {
            Settings settings;

            glm::vec4 expected = glm::vec4(-15.25f, 0.001f, 131.3f, 0.5f);
            settings.setLightPos(expected);

            Assert::AreEqual(expected.x, settings.getLightPos().x, tolerance);
            Assert::AreEqual(expected.y, settings.getLightPos().y, tolerance);
            Assert::AreEqual(expected.z, settings.getLightPos().z, tolerance);
            Assert::AreEqual(expected.w, settings.getLightPos().w, tolerance);
        }

        TEST_METHOD(SetLightConstantAttenuation_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            float withinRangeValue = 0.65f;
            settings.setLightConstantAttenuation(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getLightConstantAttenuation(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinLightConstantAttenuation() - 0.01f;
            settings.setLightConstantAttenuation(belowMinimumValue);
            Assert::AreEqual(settings.getMinLightConstantAttenuation(), settings.getLightConstantAttenuation(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxLightConstantAttenuation() + 0.01f;
            settings.setLightConstantAttenuation(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxLightConstantAttenuation(), settings.getLightConstantAttenuation(), tolerance);
        }

        TEST_METHOD(SetLightLinearAttenuation_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            float withinRangeValue = 0.65f;
            settings.setLightLinearAttenuation(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getLightLinearAttenuation(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinLightLinearAttenuation() - 0.01f;
            settings.setLightLinearAttenuation(belowMinimumValue);
            Assert::AreEqual(settings.getMinLightLinearAttenuation(), settings.getLightLinearAttenuation(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxLightLinearAttenuation() + 0.01f;
            settings.setLightLinearAttenuation(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxLightLinearAttenuation(), settings.getLightLinearAttenuation(), tolerance);
        }

        TEST_METHOD(SetLightQuadraticAttenuation_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            float withinRangeValue = 0.65f;
            settings.setLightQuadraticAttenuation(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getLightQuadraticAttenuation(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinLightQuadraticAttenuation() - 0.01f;
            settings.setLightQuadraticAttenuation(belowMinimumValue);
            Assert::AreEqual(settings.getMinLightQuadraticAttenuation(), settings.getLightQuadraticAttenuation(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxLightQuadraticAttenuation() + 0.01f;
            settings.setLightQuadraticAttenuation(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxLightQuadraticAttenuation(), settings.getLightQuadraticAttenuation(), tolerance);
        }

        TEST_METHOD(SetScaleFactor_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            float withinRangeValue = 0.029f;
            settings.setScaleFactor(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getScaleFactor(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinScaleFactor() - 0.01f;
            settings.setScaleFactor(belowMinimumValue);
            Assert::AreEqual(settings.getMinScaleFactor(), settings.getScaleFactor(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxScaleFactor() + 0.01f;
            settings.setScaleFactor(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxScaleFactor(), settings.getScaleFactor(), tolerance);
        }

        TEST_METHOD(SetIsForceColor)
        {
            Settings settings;

            // False
            bool expected = false;
            settings.setIsForceColor(expected);
            Assert::AreEqual(expected, settings.getIsForceColor());

            // True
            expected = true;
            settings.setIsForceColor(expected);
            Assert::AreEqual(expected, settings.getIsForceColor());
        }

        TEST_METHOD(SetMinForceColor_ClampsCorrectly)
        {
            Settings settings;
            settings.setMaxForceColor(100.0f);

            // Within range
            float withinRangeValue = 50.0f;
            settings.setMinForceColor(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getMinForceColor(), tolerance);

            // Below minimum
            float belowMinimumValue = -0.01f;
            settings.setMinForceColor(belowMinimumValue);
            Assert::AreEqual(0.0f, settings.getMinForceColor(), tolerance);

            // Above maximum
            float aboveMaximumValue = settings.getMaxForceColor() + 0.01f;
            settings.setMinForceColor(aboveMaximumValue);
            Assert::AreEqual(settings.getMaxForceColor(), settings.getMinForceColor(), tolerance);
        }

        TEST_METHOD(SetMaxForceColor_ClampsCorrectly)
        {
            Settings settings;
            settings.setMinForceColor(50.0f);

            // Within range
            float withinRangeValue = 75.0f;
            settings.setMaxForceColor(withinRangeValue);
            Assert::AreEqual(withinRangeValue, settings.getMaxForceColor(), tolerance);

            // Below minimum
            float belowMinimumValue = settings.getMinForceColor() - 0.01f;
            settings.setMaxForceColor(belowMinimumValue);
            Assert::AreEqual(settings.getMinForceColor(), settings.getMaxForceColor(), tolerance);
        }

        TEST_METHOD(SetBackgroundColor_ClampsCorrectly)
        {
            Settings settings;

            // Within range
            glm::vec3 withinRangeValue = glm::vec3(0.0f, 0.5f, 1.0f);
            settings.setBackgroundColor(withinRangeValue);
            Assert::AreEqual(withinRangeValue.r, settings.getBackgroundColor().r, tolerance);
            Assert::AreEqual(withinRangeValue.g, settings.getBackgroundColor().g, tolerance);
            Assert::AreEqual(withinRangeValue.b, settings.getBackgroundColor().b, tolerance);

            // Out of range
            glm::vec3 outOfRangeValue = glm::vec3(-0.1f, 0.5f, 1.1f);
            settings.setBackgroundColor(outOfRangeValue);
            Assert::AreEqual(withinRangeValue.r, settings.getBackgroundColor().r, tolerance);
            Assert::AreEqual(withinRangeValue.g, settings.getBackgroundColor().g, tolerance);
            Assert::AreEqual(withinRangeValue.b, settings.getBackgroundColor().b, tolerance);
        }
    };
}