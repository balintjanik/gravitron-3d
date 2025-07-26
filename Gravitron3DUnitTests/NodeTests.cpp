#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/Octree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(NodeTests)
    {
    public:
        TEST_METHOD(IsBranch_ShouldReturnTrue)
        {
            Node node = Node();
            node.children = 1;

            Assert::AreEqual(true, node.isBranch());
        }

        TEST_METHOD(IsBranch_ShouldReturnFalse)
        {
            Node node = Node();
            node.children = 0;

            Assert::AreEqual(false, node.isBranch());
        }

        TEST_METHOD(IsEmpty_ShouldReturnTrue)
        {
            Node node = Node();
            node.setMass(0.0f);

            Assert::AreEqual(true, node.isEmpty());
        }

        TEST_METHOD(IsEmpty_ShouldReturnFalse)
        {
            Node node = Node();
            node.setMass(10.0f);

            Assert::AreEqual(false, node.isEmpty());
        }

        TEST_METHOD(IsLeaf_ShouldReturnTrue)
        {
            Node node = Node();
            node.children = 0;

            Assert::AreEqual(true, node.isLeaf());
        }

        TEST_METHOD(IsLeaf_ShouldReturnFalse)
        {
            Node node = Node();
            node.children = 1;

            Assert::AreEqual(false, node.isLeaf());
        }
    };
}