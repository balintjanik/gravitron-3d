#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/Octree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(OctreeTests) // or OctreeTestSuite
    {
    public:
        TEST_METHOD(GetOctantFromPosition_ShouldReturn0)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(0), returnedOctantIndex);
        }

        TEST_METHOD(GetOctantFromPosition_ShouldReturn1)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(-1.0f, 1.0f, 1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(1), returnedOctantIndex);
        }

        TEST_METHOD(GetOctantFromPosition_ShouldReturn2)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(1.0f, -1.0f, 1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(2), returnedOctantIndex);
        }

        TEST_METHOD(GetOctantFromPosition_ShouldReturn3)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(-1.0f, -1.0f, 1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(3), returnedOctantIndex);
        }

        TEST_METHOD(GetOctantFromPosition_ShouldReturn4)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(1.0f, 1.0f, -1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(4), returnedOctantIndex);
        }

        TEST_METHOD(GetOctantFromPosition_ShouldReturn5)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(-1.0f, 1.0f, -1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(5), returnedOctantIndex);
        }

        TEST_METHOD(GetOctantFromPosition_ShouldReturn6)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(1.0f, -1.0f, -1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(6), returnedOctantIndex);
        }
        TEST_METHOD(GetOctantFromPosition_ShouldReturn7)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            glm::vec3 position = glm::vec3(-1.0f, -1.0f, -1.0f);
            uint32_t returnedOctantIndex = octant.getOctantFromPosition(position);
            Assert::AreEqual(static_cast<uint32_t>(7), returnedOctantIndex);
        }

        TEST_METHOD(IntoOctant_Input0)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(0);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctant_Input1)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(1);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctant_Input2)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(2);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctant_Input3)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(3);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctant_Input4)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(4);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctant_Input5)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(5);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctant_Input6)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(6);
            Assert::AreEqual(5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctant_Input7)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            Octant returnedOctant = octant.intoOctant(7);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctant.getCenter().z);
            Assert::AreEqual(5.0f, returnedOctant.getSize());
        }
        TEST_METHOD(IntoOctants_AllInputsAtOnce)
        {
            Octant octant = Octant(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f));
            std::vector<Octant> returnedOctants = octant.intoOctants();
            Assert::AreEqual(5.0f, returnedOctants[0].getCenter().x);
            Assert::AreEqual(5.0f, returnedOctants[0].getCenter().y);
            Assert::AreEqual(5.0f, returnedOctants[0].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[0].getSize());

            Assert::AreEqual(-5.0f, returnedOctants[1].getCenter().x);
            Assert::AreEqual(5.0f, returnedOctants[1].getCenter().y);
            Assert::AreEqual(5.0f, returnedOctants[1].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[1].getSize());

            Assert::AreEqual(5.0f, returnedOctants[2].getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctants[2].getCenter().y);
            Assert::AreEqual(5.0f, returnedOctants[2].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[2].getSize());

            Assert::AreEqual(-5.0f, returnedOctants[3].getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctants[3].getCenter().y);
            Assert::AreEqual(5.0f, returnedOctants[3].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[3].getSize());

            Assert::AreEqual(5.0f, returnedOctants[4].getCenter().x);
            Assert::AreEqual(5.0f, returnedOctants[4].getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctants[4].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[4].getSize());

            Assert::AreEqual(-5.0f, returnedOctants[5].getCenter().x);
            Assert::AreEqual(5.0f, returnedOctants[5].getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctants[5].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[5].getSize());

            Assert::AreEqual(5.0f, returnedOctants[6].getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctants[6].getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctants[6].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[6].getSize());

            Assert::AreEqual(-5.0f, returnedOctants[7].getCenter().x);
            Assert::AreEqual(-5.0f, returnedOctants[7].getCenter().y);
            Assert::AreEqual(-5.0f, returnedOctants[7].getCenter().z);
            Assert::AreEqual(5.0f, returnedOctants[7].getSize());
        }

        TEST_METHOD(CreateNewContaining_InputOnlyNegative)
        {
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(-10.0f, -9.0f, -8.0f, 1.0f),
                glm::vec4(  0.0f,  0.0f,  0.0f, 1.0f),
                glm::vec4(  0.0f,  0.0f,  0.0f, 0.0f),
                glm::vec4(  1.0f,  1.0f,  1.0f, 1.0f)));

            particles.push_back(Particle(
                glm::vec4(-8.0f, -7.0f, -6.0f, 1.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f, 0.0f),
                glm::vec4( 1.0f,  1.0f,  1.0f, 1.0f)));

            particles.push_back(Particle(
                glm::vec4(-7.0f, -5.5f, -4.0f, 1.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f, 0.0f),
                glm::vec4( 1.0f,  1.0f,  1.0f, 1.0f)));

            Octant returnedOctant = Octant::createNewContaining(particles);
            Assert::AreEqual(-8.50f, returnedOctant.getCenter().x);
            Assert::AreEqual(-7.25f, returnedOctant.getCenter().y);
            Assert::AreEqual(-6.00f, returnedOctant.getCenter().z);
            Assert::AreEqual( 4.00f, returnedOctant.getSize());
        }

        TEST_METHOD(CreateNewContaining_InputOnlyPositive)
        {
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(10.0f, 9.0f, 8.0f, 1.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f)));

            particles.push_back(Particle(
                glm::vec4(8.0f, 7.0f, 6.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

            particles.push_back(Particle(
                glm::vec4(4.0f, 5.5f, 7.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

            Octant returnedOctant = Octant::createNewContaining(particles);
            Assert::AreEqual(7.00f, returnedOctant.getCenter().x);
            Assert::AreEqual(7.25f, returnedOctant.getCenter().y);
            Assert::AreEqual(7.00f, returnedOctant.getCenter().z);
            Assert::AreEqual(6.00f, returnedOctant.getSize());
        }

        TEST_METHOD(CreateNewContaining_InputPositiveAndNegative)
        {
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(-10.0f, 9.0f, -2.0f, 1.0f),
                glm::vec4(  0.0f, 0.0f,  0.0f, 1.0f),
                glm::vec4(  0.0f, 0.0f,  0.0f, 0.0f),
                glm::vec4(  1.0f, 1.0f,  1.0f, 1.0f)));

            particles.push_back(Particle(
                glm::vec4(10.0f, 2.0f, 6.0f, 1.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f)));

            particles.push_back(Particle(
                glm::vec4(2.0f, -5.5f, -7.0f, 1.0f),
                glm::vec4(0.0f,  0.0f,  0.0f, 1.0f),
                glm::vec4(0.0f,  0.0f,  0.0f, 0.0f),
                glm::vec4(1.0f,  1.0f,  1.0f, 1.0f)));

            Octant returnedOctant = Octant::createNewContaining(particles);
            Assert::AreEqual( 0.00f, returnedOctant.getCenter().x);
            Assert::AreEqual( 1.75f, returnedOctant.getCenter().y);
            Assert::AreEqual(-0.50f, returnedOctant.getCenter().z);
            Assert::AreEqual(20.00f, returnedOctant.getSize());
        }

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

        TEST_METHOD(Clear_ShouldBecomeEmpty)
        {
            Octree octree;
            Node node1 = Node();
            node1.children = 1;
            octree.nodes.push_back(node1);
            octree.parents.push_back(0);
            Node node2 = Node();
            node2.children = 0;
            octree.nodes.push_back(node2);
            Node node3 = Node();
            node3.children = 2;
            octree.nodes.push_back(node3);
            octree.parents.push_back(2);

            Assert::AreEqual(static_cast<size_t>(3), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(2), octree.parents.size());

            Octant octant = Octant::createNewContaining(std::vector<Particle>());
            octree.clear(octant);

            Assert::AreEqual(static_cast<size_t>(1), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(0), octree.parents.size());
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getSize());
        }
    };
}