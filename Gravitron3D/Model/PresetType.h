#pragma once

enum PresetType {
	PRESET_GALAXY,
	PRESET_SOLAR_SYSTEM,
	PRESET_GALAXY_COLLISION,
	PRESET_RANDOM,
	PRESET_CUSTOM
};

static const char* PRESET_TYPE_NAMES[] = {
	"Galaxy",
	"Solar system",
	"Galaxy collision",
	"Random",
	"Custom"
};