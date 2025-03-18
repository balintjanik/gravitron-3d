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