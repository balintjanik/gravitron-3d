#pragma once

enum VelocityType {
	VELOCITY_RANDOM,
	VELOCITY_ZERO,
	VELOCITY_ORBIT
};

static const char* VELOCITY_TYPE_NAMES[] = {
	"Random",
	"Zero",
	"Orbit"
};

struct VelocityProperties {
	VelocityType type = VelocityType::VELOCITY_ZERO;
	float scale = 1.0f;
	glm::vec3 randomMin = glm::vec3(0.0f);
	glm::vec3 randomMax = glm::vec3(0.0f);
	glm::vec3 overallVelocity = glm::vec3(0.0f);
};