#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Utils
#include "GLUtils.hpp"
#include "Camera.h"
#include "CameraManipulator.h"

#include "../Model/SimulationManager.h"
#include "../Model/SUpdateInfo.h"
#include <thread>

class SimulationView
{
public:
	SimulationView();
	~SimulationView();

	bool Init();
	void Clean();

	void Update( const SUpdateInfo& );
	void Render();
	void RenderGUI();

	void KeyboardDown(const SDL_KeyboardEvent&);
	void KeyboardUp(const SDL_KeyboardEvent&);
	void MouseMove(const SDL_MouseMotionEvent&);
	void MouseDown(const SDL_MouseButtonEvent&);
	void MouseUp(const SDL_MouseButtonEvent&);
	void MouseWheel(const SDL_MouseWheelEvent&);
	void Resize(int, int);

	void OtherEvent( const SDL_Event& );
protected:
	void SetupDebugCallback();

	float m_ElapsedTimeInSec = 0.0f;
	float m_DeltaTimeInSec = 0.0f;

	// Camera
	Camera m_camera;
	CameraManipulator m_cameraManipulator;

	// Shaders
	GLuint m_programID = 0;

	void InitShaders();
	void CleanShaders();

	// Light
	glm::vec4 m_lightPos = glm::vec4( 0.0f, -1.0f, 0.0f, 0.0f );

	float m_lightConstantAttenuation    = 0.0;
	float m_lightLinearAttenuation      = 1.0;
	float m_lightQuadraticAttenuation   = 0.0;
	
	// Geometry
	float scaleFactor = 0.005f; // TODO: move to model?
	OGLObject m_sphereGPU = {};
	GLuint  instanceVBO = 0;

	void InitGeometry();
	void CleanGeometry();

	// FPS
	unsigned int fps = 0;
	unsigned int frames;
	float timeSinceLastSec = 0.0f;

	// Textures
    GLuint m_SamplerID = 0;
	GLuint m_sphereTextureID = 0;

	void InitTextures();
	void CleanTextures();

	// Model
	SimulationManager simulationManager;
	std::vector<glm::vec4> particlePositions;
};

