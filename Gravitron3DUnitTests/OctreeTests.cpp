#include "pch.h"
#include "CppUnitTest.h"
#include "../Gravitron3D/Model/Octree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Gravitron3DUnitTests
{
    TEST_CLASS(OctreeTests)
    {
    public:

        const float tolerance = 0.0001f;
        
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

            std::vector<Particle> particles;
            Octant octant = Octant::createNewContaining(particles);
            octree.clear(octant);

            Assert::AreEqual(static_cast<size_t>(1), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(0), octree.parents.size());
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getSize());
        }

        TEST_METHOD(Subdivide_CheckAllChildrenData)
        {
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(-5.0f,-5.0f, -5.0f, 1.0f),
                glm::vec4( 0.0f, 0.0f,  0.0f, 1.0f),
                glm::vec4( 0.0f, 0.0f,  0.0f, 0.0f),
                glm::vec4( 1.0f, 1.0f,  1.0f, 1.0f)));

            particles.push_back(Particle(
                glm::vec4(5.0f, 5.0f, 5.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

            Octree octree;
            Octant octant = Octant::createNewContaining(particles);
            octree.clear(octant);

            Assert::AreEqual(static_cast<size_t>(1), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(0), octree.parents.size());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[0].children);
            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[0].next);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(10.0f, octree.nodes[0].octant.getSize());

            octree.subdivide(0);

            Assert::AreEqual(static_cast<size_t>(9), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(1), octree.parents.size());

            Assert::AreEqual(static_cast<uint32_t>(1), octree.nodes[0].children);
            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[0].next);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(0.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(10.0f, octree.nodes[0].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[1].children);
            Assert::AreEqual(static_cast<uint32_t>(2), octree.nodes[1].next);
            Assert::AreEqual(5.0f, octree.nodes[1].octant.getCenter().x);
            Assert::AreEqual(5.0f, octree.nodes[1].octant.getCenter().y);
            Assert::AreEqual(5.0f, octree.nodes[1].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[1].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[2].children);
            Assert::AreEqual(static_cast<uint32_t>(3), octree.nodes[2].next);
            Assert::AreEqual(-5.0f, octree.nodes[2].octant.getCenter().x);
            Assert::AreEqual(5.0f, octree.nodes[2].octant.getCenter().y);
            Assert::AreEqual(5.0f, octree.nodes[2].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[2].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[3].children);
            Assert::AreEqual(static_cast<uint32_t>(4), octree.nodes[3].next);
            Assert::AreEqual(5.0f, octree.nodes[3].octant.getCenter().x);
            Assert::AreEqual(-5.0f, octree.nodes[3].octant.getCenter().y);
            Assert::AreEqual(5.0f, octree.nodes[3].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[3].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[4].children);
            Assert::AreEqual(static_cast<uint32_t>(5), octree.nodes[4].next);
            Assert::AreEqual(-5.0f, octree.nodes[4].octant.getCenter().x);
            Assert::AreEqual(-5.0f, octree.nodes[4].octant.getCenter().y);
            Assert::AreEqual(5.0f, octree.nodes[4].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[4].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[5].children);
            Assert::AreEqual(static_cast<uint32_t>(6), octree.nodes[5].next);
            Assert::AreEqual(5.0f, octree.nodes[5].octant.getCenter().x);
            Assert::AreEqual(5.0f, octree.nodes[5].octant.getCenter().y);
            Assert::AreEqual(-5.0f, octree.nodes[5].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[5].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[6].children);
            Assert::AreEqual(static_cast<uint32_t>(7), octree.nodes[6].next);
            Assert::AreEqual(-5.0f, octree.nodes[6].octant.getCenter().x);
            Assert::AreEqual(5.0f, octree.nodes[6].octant.getCenter().y);
            Assert::AreEqual(-5.0f, octree.nodes[6].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[6].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[7].children);
            Assert::AreEqual(static_cast<uint32_t>(8), octree.nodes[7].next);
            Assert::AreEqual(5.0f, octree.nodes[7].octant.getCenter().x);
            Assert::AreEqual(-5.0f, octree.nodes[7].octant.getCenter().y);
            Assert::AreEqual(-5.0f, octree.nodes[7].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[7].octant.getSize());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[8].children);
            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[8].next);
            Assert::AreEqual(-5.0f, octree.nodes[8].octant.getCenter().x);
            Assert::AreEqual(-5.0f, octree.nodes[8].octant.getCenter().y);
            Assert::AreEqual(-5.0f, octree.nodes[8].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[8].octant.getSize());
        }

        TEST_METHOD(Insert_CheckAllNodesData)
        {
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(10.0f, 20.0f, 30.0f, 100.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f,   1.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f,   0.0f)
            ));
            particles.push_back(Particle(
                glm::vec4(20.0f, 30.0f, 40.0f, 50.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f,  1.0f),
                glm::vec4( 0.0f,  0.0f,  0.0f,  0.0f)
            ));

            Octree octree;
            Octant octant = Octant::createNewContaining(particles);
            octree.clear(octant);

            Assert::AreEqual(static_cast<size_t>(1), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(0), octree.parents.size());

            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[0].children);
            Assert::AreEqual(static_cast<uint32_t>(0), octree.nodes[0].next);
            Assert::AreEqual(0.0f, octree.nodes[0].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[0].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[0].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[0].getMass());
            Assert::AreEqual(15.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(25.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(35.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(10.0f, octree.nodes[0].octant.getSize());

            octree.insert(particles[0].getPosition(), particles[0].getMass());

            Assert::AreEqual(static_cast<size_t>(1), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(0), octree.parents.size());
            Assert::AreEqual(10.0f, octree.nodes[0].getPosition().x);
            Assert::AreEqual(20.0f, octree.nodes[0].getPosition().y);
            Assert::AreEqual(30.0f, octree.nodes[0].getPosition().z);
            Assert::AreEqual(100.0f, octree.nodes[0].getMass());
            Assert::AreEqual(15.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(25.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(35.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(10.0f, octree.nodes[0].octant.getSize());

            octree.insert(particles[1].getPosition(), particles[1].getMass());

            Assert::AreEqual(static_cast<size_t>(9), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(1), octree.parents.size());
            Assert::AreEqual(10.0f, octree.nodes[0].getPosition().x);
            Assert::AreEqual(20.0f, octree.nodes[0].getPosition().y);
            Assert::AreEqual(30.0f, octree.nodes[0].getPosition().z);
            Assert::AreEqual(100.0f, octree.nodes[0].getMass());
            Assert::AreEqual(15.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(25.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(35.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(10.0f, octree.nodes[0].octant.getSize());

            Assert::AreEqual(20.0f, octree.nodes[1].getPosition().x);
            Assert::AreEqual(30.0f, octree.nodes[1].getPosition().y);
            Assert::AreEqual(40.0f, octree.nodes[1].getPosition().z);
            Assert::AreEqual(50.0f, octree.nodes[1].getMass());
            Assert::AreEqual(20.0f, octree.nodes[1].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[1].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[1].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[1].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[2].getMass());
            Assert::AreEqual(10.0f, octree.nodes[2].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[2].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[2].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[2].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[3].getMass());
            Assert::AreEqual(20.0f, octree.nodes[3].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[3].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[3].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[3].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[4].getMass());
            Assert::AreEqual(10.0f, octree.nodes[4].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[4].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[4].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[4].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[5].getMass());
            Assert::AreEqual(20.0f, octree.nodes[5].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[5].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[5].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[5].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[6].getMass());
            Assert::AreEqual(10.0f, octree.nodes[6].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[6].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[6].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[6].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[7].getMass());
            Assert::AreEqual(20.0f, octree.nodes[7].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[7].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[7].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[7].octant.getSize());

            Assert::AreEqual(10.0f, octree.nodes[8].getPosition().x);
            Assert::AreEqual(20.0f, octree.nodes[8].getPosition().y);
            Assert::AreEqual(30.0f, octree.nodes[8].getPosition().z);
            Assert::AreEqual(100.0f, octree.nodes[8].getMass());
            Assert::AreEqual(10.0f, octree.nodes[8].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[8].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[8].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[8].octant.getSize());
        }

        TEST_METHOD(Propagate_CheckAllNodesData)
        {
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(10.0f, 20.0f, 30.0f, 100.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
            ));
            particles.push_back(Particle(
                glm::vec4(20.0f, 30.0f, 40.0f, 50.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
            ));

            Octree octree;
            Octant octant = Octant::createNewContaining(particles);
            octree.clear(octant);

            octree.insert(particles[0].getPosition(), particles[0].getMass());
            octree.insert(particles[1].getPosition(), particles[1].getMass());

            Assert::AreEqual(static_cast<size_t>(9), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(1), octree.parents.size());
            Assert::AreEqual(10.0f, octree.nodes[0].getPosition().x);
            Assert::AreEqual(20.0f, octree.nodes[0].getPosition().y);
            Assert::AreEqual(30.0f, octree.nodes[0].getPosition().z);
            Assert::AreEqual(100.0f, octree.nodes[0].getMass());
            Assert::AreEqual(15.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(25.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(35.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(10.0f, octree.nodes[0].octant.getSize());

            Assert::AreEqual(20.0f, octree.nodes[1].getPosition().x);
            Assert::AreEqual(30.0f, octree.nodes[1].getPosition().y);
            Assert::AreEqual(40.0f, octree.nodes[1].getPosition().z);
            Assert::AreEqual(50.0f, octree.nodes[1].getMass());
            Assert::AreEqual(20.0f, octree.nodes[1].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[1].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[1].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[1].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[2].getMass());
            Assert::AreEqual(10.0f, octree.nodes[2].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[2].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[2].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[2].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[3].getMass());
            Assert::AreEqual(20.0f, octree.nodes[3].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[3].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[3].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[3].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[4].getMass());
            Assert::AreEqual(10.0f, octree.nodes[4].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[4].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[4].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[4].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[5].getMass());
            Assert::AreEqual(20.0f, octree.nodes[5].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[5].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[5].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[5].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[6].getMass());
            Assert::AreEqual(10.0f, octree.nodes[6].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[6].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[6].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[6].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[7].getMass());
            Assert::AreEqual(20.0f, octree.nodes[7].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[7].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[7].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[7].octant.getSize());

            Assert::AreEqual(10.0f, octree.nodes[8].getPosition().x);
            Assert::AreEqual(20.0f, octree.nodes[8].getPosition().y);
            Assert::AreEqual(30.0f, octree.nodes[8].getPosition().z);
            Assert::AreEqual(100.0f, octree.nodes[8].getMass());
            Assert::AreEqual(10.0f, octree.nodes[8].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[8].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[8].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[8].octant.getSize());

            octree.propagate();

            Assert::AreEqual(static_cast<size_t>(9), octree.nodes.size());
            Assert::AreEqual(static_cast<size_t>(1), octree.parents.size());
            Assert::AreEqual(13.333333f, octree.nodes[0].getPosition().x, tolerance);
            Assert::AreEqual(23.333333f, octree.nodes[0].getPosition().y, tolerance);
            Assert::AreEqual(33.333333f, octree.nodes[0].getPosition().z, tolerance);
            Assert::AreEqual(150.0f, octree.nodes[0].getMass());
            Assert::AreEqual(15.0f, octree.nodes[0].octant.getCenter().x);
            Assert::AreEqual(25.0f, octree.nodes[0].octant.getCenter().y);
            Assert::AreEqual(35.0f, octree.nodes[0].octant.getCenter().z);
            Assert::AreEqual(10.0f, octree.nodes[0].octant.getSize());

            Assert::AreEqual(20.0f, octree.nodes[1].getPosition().x);
            Assert::AreEqual(30.0f, octree.nodes[1].getPosition().y);
            Assert::AreEqual(40.0f, octree.nodes[1].getPosition().z);
            Assert::AreEqual(50.0f, octree.nodes[1].getMass());
            Assert::AreEqual(20.0f, octree.nodes[1].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[1].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[1].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[1].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[2].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[2].getMass());
            Assert::AreEqual(10.0f, octree.nodes[2].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[2].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[2].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[2].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[3].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[3].getMass());
            Assert::AreEqual(20.0f, octree.nodes[3].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[3].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[3].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[3].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[4].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[4].getMass());
            Assert::AreEqual(10.0f, octree.nodes[4].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[4].octant.getCenter().y);
            Assert::AreEqual(40.0f, octree.nodes[4].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[4].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[5].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[5].getMass());
            Assert::AreEqual(20.0f, octree.nodes[5].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[5].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[5].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[5].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[6].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[6].getMass());
            Assert::AreEqual(10.0f, octree.nodes[6].octant.getCenter().x);
            Assert::AreEqual(30.0f, octree.nodes[6].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[6].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[6].octant.getSize());

            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().x);
            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().y);
            Assert::AreEqual(0.0f, octree.nodes[7].getPosition().z);
            Assert::AreEqual(0.0f, octree.nodes[7].getMass());
            Assert::AreEqual(20.0f, octree.nodes[7].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[7].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[7].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[7].octant.getSize());

            Assert::AreEqual(10.0f, octree.nodes[8].getPosition().x);
            Assert::AreEqual(20.0f, octree.nodes[8].getPosition().y);
            Assert::AreEqual(30.0f, octree.nodes[8].getPosition().z);
            Assert::AreEqual(100.0f, octree.nodes[8].getMass());
            Assert::AreEqual(10.0f, octree.nodes[8].octant.getCenter().x);
            Assert::AreEqual(20.0f, octree.nodes[8].octant.getCenter().y);
            Assert::AreEqual(30.0f, octree.nodes[8].octant.getCenter().z);
            Assert::AreEqual(5.0f, octree.nodes[8].octant.getSize());
        }

        TEST_METHOD(CalculateAcceleration_CheckAllParticlesData)
        {
            std::vector<Particle> particles;
            particles.push_back(Particle(
                glm::vec4(10.0f,0.0f, 0.0f, 100.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f,  1.0f),
                glm::vec4( 0.0f, 0.0f, 0.0f,  0.0f)
            ));
            particles.push_back(Particle(
                glm::vec4(-10.0f, 0.0f, 0.0f, 100.0f),
                glm::vec4(  0.0f, 0.0f, 0.0f,   1.0f),
                glm::vec4(  0.0f, 0.0f, 0.0f,   0.0f)
            ));

            Octree octree;
            Octant octant = Octant::createNewContaining(particles);
            octree.clear(octant);

            octree.insert(particles[0].getPosition(), particles[0].getMass());
            octree.insert(particles[1].getPosition(), particles[1].getMass());
            
            octree.propagate();

            float expectedAccelerations[] = {
                -0.25f, 0.0f, 0.0f,
                0.25f, 0.0f, 0.0f
            };

            float expectedForces[] = {
                0.25f,
                0.25f
            };

            for (int i = 0; i < particles.size(); i++) {
                Particle& p = particles[i];
                glm::vec3 acceleration = glm::vec3(0);
                float allForce = octree.calculateAcceleration(acceleration, p.getPosition(), 0.0f, 0.0f);

                Assert::AreEqual(expectedAccelerations[(i*3)], acceleration.x, tolerance);
                Assert::AreEqual(expectedAccelerations[(i*3)+1], acceleration.y, tolerance);
                Assert::AreEqual(expectedAccelerations[(i * 3) + 2], acceleration.z, tolerance);
                Assert::AreEqual(expectedForces[i], allForce, tolerance);
            }
        }
    };
}