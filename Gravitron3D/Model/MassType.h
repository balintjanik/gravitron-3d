#pragma once

enum MassType {
	MASS_CONSTANT,
	MASS_RANDOM
};

static const char* MASS_TYPE_NAMES[] = {
	"Constant",
	"Random"
};

struct MassProperties {
	MassType type = MassType::MASS_CONSTANT;
	float value = 1.0f;
	float randomMin = 1.0f;
	float randomMax = 1.0f;
};