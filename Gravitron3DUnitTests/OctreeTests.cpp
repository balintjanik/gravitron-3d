#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/Octree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(OctreeTests) // or OctreeTestSuite
    {
    public:
        TEST_METHOD(GetOctantFromPosition_ShouldReturn7)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(-1.0f, -1.0f, -1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(7), returnedOctantIndex);
        }
    };
}