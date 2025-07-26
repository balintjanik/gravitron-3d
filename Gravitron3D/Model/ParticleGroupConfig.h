#pragma once

#include "PositionType.h"
#include "VelocityType.h"
#include "MassType.h"
#include "SizeType.h"

struct ParticleGroupConfig {
    int numberOfParticlesToAdd = 0;
    PositionType position = PositionType::POSITION_SPHERE;
    SpawnRegion region;
    VelocityProperties velocity;
    bool movable = true;
    float centerMass = 1.0f;
    MassProperties mass;
    SizeProperties size;
};