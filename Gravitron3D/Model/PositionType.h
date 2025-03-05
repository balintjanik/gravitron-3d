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