#pragma once

enum SizeType {
	SIZE_CONSTANT,
	SIZE_RANDOM
};

static const char* SIZE_TYPE_NAMES[] = {
	"Constant",
	"Random"
};

struct SizeProperties {
	SizeType type = SizeType::SIZE_CONSTANT;
	float value = 1.0f;
	float randomMin = 1.0f;
	float randomMax = 1.0f;
};