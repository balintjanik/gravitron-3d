#include "particle.h"
#include <random>
#include <iostream>

float randomFloat(float min, float max) {
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void Particle::update(float acc_x, float acc_y, float acc_z, float delta_time, float speed, float all_force)
{
	this->acc_x = acc_x;
	this->acc_y = acc_y;
	this->acc_z = acc_z;

	vel_x += acc_x * delta_time * speed;
	vel_y += acc_y * delta_time * speed;
	vel_z += acc_z * delta_time * speed;

	pos_x += vel_x * delta_time * speed;
	pos_y += vel_y * delta_time * speed;
	pos_z += vel_z * delta_time * speed;

	if (pos_x < -3000 || pos_x > 3000)
		vel_x *= -1;
	if (pos_y < -3000 || pos_y > 3000)
		vel_y *= -1;
	if (pos_z < -3000 || pos_z > 3000)
		vel_z *= -1;

	this->all_force = all_force;
}

std::vector<Particle> initParticles(int n)
{
	srand((unsigned)time(0));

	std::vector<Particle> particles;
	float centerX = 0.f;
	float centerY = 0.f;
	float centerZ = 0.f;
	float radiusMin = 20.0f;
	float radiusMax = 300.0f;
	float velocity_scale = 0.8f;

	float center_mass = 100000.0f;
	particles.push_back(Particle(centerX, centerY, centerZ, 0.0f, 0.0f, 0.0f, center_mass));
	for (int i = 1; i < n; i++)
	{

		// Generate a random position in a sphere
		float radius = randomFloat(radiusMin, radiusMax); // Random radius between radiusMin and radiusMax
		float theta = randomFloat(0.0f, 2 * 3.1415f);     // Random angle theta (0 to 2π)
		float phi = randomFloat(0.0f, 3.1415f);           // Random angle phi (0 to π)

		// Convert spherical to Cartesian coordinates for position
		float px = centerX + radius * sin(phi) * cos(theta);
		float py = centerY + radius * sin(phi) * sin(theta);
		float pz = centerZ + radius * cos(phi);

		// Calculate the 3D distance from the center to the particle
		float distance = sqrt((px - centerX) * (px - centerX) + (py - centerY) * (py - centerY) + (pz - centerZ) * (pz - centerZ));

		// Calculate initial velocity magnitude for circular rotation, based on 3D distance
		float velocityMagnitude = sqrt(center_mass / distance) * velocity_scale; // Adjust this based on desired rotation speed

		// Determine a perpendicular vector for the initial velocity
		// Here, we use a simple approach by crossing the radius vector with an arbitrary vector (1, 0, 0) to get a perpendicular direction
		float rx = px - centerX;
		float ry = py - centerY;
		float rz = pz - centerZ;

		// Use a consistent reference vector for the cross product, e.g., (0, 1, 0)
		float ref_x = 0.0f;
		float ref_y = 1.0f;
		float ref_z = 0.0f;

		// Cross product of radius vector (rx, ry, rz) with reference vector (ref_x, ref_y, ref_z)
		float vx = ry * ref_z - rz * ref_y;
		float vy = rz * ref_x - rx * ref_z;
		float vz = rx * ref_y - ry * ref_x;

		// Normalize the perpendicular vector
		float length = sqrt(vx * vx + vy * vy + vz * vz);
		vx = (vx / length) * velocityMagnitude;
		vy = (vy / length) * velocityMagnitude;
		vz = (vz / length) * velocityMagnitude;

		// Calculate initial velocity for circular rotation
		// float velocityMagnitude = sqrt(center_mass / radius) * velocity_scale; // Adjust this based on desired rotation speed
		// float vx = velocityMagnitude * (py - centerY) / radius;  // Perpendicular to radius vector
		// float vy = -velocityMagnitude * (px - centerX) / radius;
		// float vz = 0.0f;  // Initial velocity in Z to maintain rotation in X-Y plane around the center

		float mass = 1.0f; // Set particle mass

		// Add particle to the list
		particles.push_back(Particle(px, py, pz, vx, vy, vz, mass));
	}

	/*std::default_random_engine generator;
	std::uniform_real_distribution<float> radius_dist(50.0f, 500.0f); // Radius between 50 and 500
	std::uniform_real_distribution<float> angle_dist(0.0f, 2 * 3.1415f); // Angle from 0 to 2π
	float mean_z = 550.0f;
	float stddev_z = 30.0f;
	std::normal_distribution<float> z_dist(mean_z, stddev_z); // Normal distribution for z scatter

	std::vector<Particle> particles;
	float center_mass = 1000000.0;
	particles.push_back(Particle(550.0f, 550.0f, 550.0f, 0.0f, 0.0f, 0.0f, center_mass));
	for (int i = 0; i < n; i++)
	{
		// Random position in spherical coordinates
		float radius = radius_dist(generator);
		float angle_xy = angle_dist(generator); // Angle in the xy-plane

		// Polar to Cartesian conversion with a slight tilt in the z-axis
		float px = 550.0f + radius * cos(angle_xy);
		float py = 550.0f + radius * sin(angle_xy);
		float pz = z_dist(generator); // Small scatter in z direction

		// Radial vector from the center to the particle
		float dx = px - 550.0f;
		float dy = py - 550.0f;
		float dz = pz - mean_z;

		// Calculate the radial distance
		float radial_distance = sqrt(dx * dx + dy * dy + dz * dz);

		// Velocity magnitude for circular orbit
		float velocityMagnitude = sqrt(center_mass / radial_distance);

		// Velocity direction: perpendicular to radial vector
		// Use an arbitrary perpendicular vector via cross product with a fixed reference (e.g., z-axis)
		float vx = velocityMagnitude * (-dy / sqrt(dx * dx + dy * dy));
		float vy = velocityMagnitude * (dx / sqrt(dx * dx + dy * dy));
		float vz = 0.0f; // Initial vz can be zero if we want orbit primarily in the xy-plane

		float m = 1.0f; // Particle mass

		// Add the particle to the vector
		particles.push_back(Particle(px, py, pz, vx, vy, vz, m));
	}*/

	/*std::vector<Particle> particles;
	float center_mass = 1000000.0;
	particles.push_back(Particle(550.0f, 550.0f, 550.0f, 0.0f, 0.0f, 0.0f, center_mass));
	for (int i = 1; i < n; i++)
	{
		/*float px = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1920.0));
		float py = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1080.0));
		float vx = 0;// -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - (-1.0))));
		float vy = 0;// -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0 - (-1.0))));
		float m = 1.0;// static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 5.0));
		particles.push_back(Particle(px, py, vx, vy, m));*//*

		// Random position around the center
		float radius = 50.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (500 - 50))); // Random radius between 100 and 300
		float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (2 * 3.1415); // Random angle

		float height = -10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (10 - (-10)))); // Random height between -10 and +10

		// Calculate particle position based on polar coordinates
		float px = 550.0f + radius * cos(angle);
		float py = 550.0f + radius * sin(angle);
		float pz = 550.0f + height;

		// Calculate initial velocity to create circular motion
		float temperature_x = -10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (10 - (-10))));
		float temperature_y = -10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (10 - (-10))));
		float temperature_z = -10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (10 - (-10))));
		float velocityMagnitude = sqrt(center_mass / radius); // Adjust this based on your needs
		float vx = temperature_x;//-velocityMagnitude * sin(angle);// + temperature_x; // Perpendicular to radius
		float vy = temperature_y;//velocityMagnitude * cos(angle);// + temperature_y; // Perpendicular to radius
		float vz = 0.0f + temperature_z;

		float m = 1.0f; // Set particle mass

		particles.push_back(Particle(px, py, pz, vx, vy, vz, m));
	}*/
	/*float center_mass = 1000000.0;
	float svx = 0.0f;
	float svy = 0.0f;
	float svz = 0.0f;
	float spx = 350.0f;
	float spy = 700.0f;
	float spz = 550.0f;
	particles.push_back(Particle(spx, spy, spz, svx, svy, svz, center_mass));
	for (int i = 1; i < 2 * n / 3; i++)
	{
		// Random position around the center
		float radius = 20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (350 - 20))); // Random radius between 100 and 300
		float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (2 * 3.1415); // Random angle

		float height = -50.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (50 - (-50)))); // Random height between -100 and +100

		// Calculate particle position based on polar coordinates
		float px = spx + radius * cos(angle);
		float py = spy + radius * sin(angle);
		float pz = spz + height;

		// Calculate initial velocity to create circular motion
		float velocityMagnitude = sqrt(center_mass / radius); // Adjust this based on your needs
		float vx = -velocityMagnitude * sin(angle) + svx; // Perpendicular to radius
		float vy = velocityMagnitude * cos(angle) + svy; // Perpendicular to radius
		float vz = 0.0f;

		float m = 1.0f; // Set particle mass

		particles.push_back(Particle(px, py, pz, vx, vy, vz, m));
	}

	center_mass = 500000.0;
	svx = -20.0f;
	svy = 0.0f;
	svz = 0.0f;
	spx = 1400.0f;
	spy = 200.0f;
	spz = 550.0f;
	particles.push_back(Particle(spx, spy, spz, svx, svy, svz, center_mass));
	for (int i = 2 * n / 3 + 1; i < n; i++)
	{
		// Random position around the center
		float radius = 20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (170 - 20))); // Random radius between 100 and 300
		float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (2 * 3.1415); // Random angle

		float height = -50.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (50 - (-50)))); // Random height between -100 and +100

		// Calculate particle position based on polar coordinates
		float px = spx + radius * cos(angle);
		float py = spy + radius * sin(angle);
		float pz = spz +height;

		// Calculate initial velocity to create circular motion
		float velocityMagnitude = sqrt(center_mass / radius); // Adjust this based on your needs
		float vx = -velocityMagnitude * sin(angle) + svx; // Perpendicular to radius
		float vy = velocityMagnitude * cos(angle) + svy; // Perpendicular to radius
		float vz = 0.0f;

		float m = 1.0f; // Set particle mass

		particles.push_back(Particle(px, py, pz, vx, vy, vz, m));
	}*/

	return particles;
}
