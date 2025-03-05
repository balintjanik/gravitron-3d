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

// ImGui
#include <imgui.h>

// Utils
#include "GLUtils.hpp"
#include "Camera.h"
#include "CameraManipulator.h"

// Model
#include "../Model/SimulationManager.h"
#include "../Model/SUpdateInfo.h"

class SimulationView
{
public:
	SimulationView();
	~SimulationView();

	bool Init();
	void Clean();
	
	void UpdateData();
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
	
	// Geometry
	OGLObject m_sphereGPU = {};
	GLuint  instanceVBO = 0;

	void InitGeometry();
	void CleanGeometry();

	// Textures
    GLuint m_SamplerID = 0;
	GLuint m_sphereTextureID = 0;

	void InitTextures();
	void CleanTextures();

	// ImGui
	void InitImGuiSettings();
	ImGuiIO& io = ImGui::GetIO();
	float windowWidth;
	float windowHeight;

	// Model
	SimulationManager simulationManager;
	std::vector<glm::vec4> particlePositions;

	// Settings
	float theta = 1.0f;
	float epsilon = 1.0f;

	// Performance
	unsigned int fps = 0;
	unsigned int frames;
	float timeSinceLastSec = 0.0f;
	uint32_t numberOfThreads = 1;

	// Current simulation info
	int currentNumberOfParticles;

	// Light settings
	glm::vec4 m_lightPos = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

	float m_lightConstantAttenuation = 0.0;
	float m_lightLinearAttenuation = 1.0;
	float m_lightQuadraticAttenuation = 0.0;

	// Display settings
	float scaleFactor = 0.005f; // TODO: move to model?
	float simulationSpeed = 1.0f;

	// New simulation settings
	int numberOfParticles = 80000;
	PresetType presetType = static_cast<PresetType>(0);
	PositionType positionType = static_cast<PositionType>(0);
};

