#pragma once

#include <glm/glm.hpp>

class Camera;

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseWheelEvent;

class CameraManipulator
{
public:
    CameraManipulator();

    ~CameraManipulator();

	void SetCamera( Camera* _pCamera );
    void Update( float _deltaTime );

	inline void  SetSpeed(float _speed) {
		m_speed = _speed >= MIN_SPEED ? _speed : MIN_SPEED;
		m_speed = m_speed <= MAX_SPEED ? _speed : MAX_SPEED;
	}
	inline float GetSpeed() const noexcept { return m_speed; }
	inline float GetMinSpeed() const noexcept { return MIN_SPEED; }
	inline float GetMaxSpeed() const noexcept { return MAX_SPEED; }
	inline void  SetDistance(float _distance) {
		m_distance = _distance >= MIN_DISTANCE ? _distance : MIN_DISTANCE;
		m_distance = m_distance <= MAX_DISTANCE ? _distance : MAX_DISTANCE;
	}
	inline float GetDistance() const noexcept { return m_distance; }
	inline float GetMinDistance() const noexcept { return MIN_DISTANCE; }
	inline float GetMaxDistance() const noexcept { return MAX_DISTANCE; }

	void KeyboardDown(const SDL_KeyboardEvent& key);
	void KeyboardUp(const SDL_KeyboardEvent& key);
	void MouseMove(const SDL_MouseMotionEvent& mouse);
	void MouseWheel(const SDL_MouseWheelEvent& wheel );

private:
    Camera* m_pCamera = nullptr;

	// The u spherical coordinate of the spherical coordinate pair (u,v) denoting the
	// current viewing direction from the view position m_eye. 
	float	m_u = 0.0f;

	// The v spherical coordinate of the spherical coordinate pair (u,v) denoting the
	// current viewing direction from the view position m_eye. 
	float	m_v = 0.0f;

	// The distance of the look at point from the camera. 
	float	m_distance = 0.0f;
	const float MIN_DISTANCE = 0.015f;
	const float MAX_DISTANCE = 100.0f;

	// The center of model sphere.
	glm::vec3 m_center = glm::vec3( 0.0f );

	// The traversal speed of the camera
	float m_speed = 8.0f;
	const float MIN_SPEED = 1.0f;
	const float MAX_SPEED = 50.0f;

	// Traveling indicator to different directions.
	float	m_goForward = 0.0f;
	float	m_goRight   = 0.0f;
	float   m_goUp      = 0.0f;
};