#pragma once

enum PositionType {
	POSITION_RANDOM,
	POSITION_SPHERE,
	POSITION_DISK,
	POSITION_GRID_3D,
	POSITION_GRID_2D
};

static const char* POSITION_TYPE_NAMES[] = {
	"Random",
	"Sphere",
	"Disk",
	"Grid 3D",
	"Grid 2D"
};

struct SpawnRegion {
	glm::vec3 cubeMin = glm::vec3(0.0f);
	glm::vec3 cubeMax = glm::vec3(0.0f);
	glm::vec3 sphereCenter = glm::vec3(0.0f);
	float sphereRadiusMin = 25.0f;
	float sphereRadiusMax = 250.0f;
};