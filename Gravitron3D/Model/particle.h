#pragma once
#include <vector>

struct Particle {
	float pos_x, pos_y, pos_z;
	float vel_x, vel_y, vel_z;
	float acc_x, acc_y, acc_z;
	float mass;
	float all_force = 0.0f;
	Particle(float px = 0.0, float py = 0.0, float pz = 0.0, float vx = 0.0, float vy = 0.0, float vz = 0.0, float m = 1.0, float ax = 0.0, float ay = 0.0, float az = 0.0)
		: pos_x(px), pos_y(py), pos_z(pz), vel_x(vx), vel_y(vy), vel_z(vz), mass(m), acc_x(ax), acc_y(ay), acc_z(az)
	{
	}

	void update(float acc_x, float acc_y, float acc_z, float delta_time, float speed, float all_force);
};

std::vector<Particle> initParticles(int n);
