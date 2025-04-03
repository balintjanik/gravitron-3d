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
#include <string>

// Utils
#include "GLUtils.hpp"
#include "Camera.h"
#include "CameraManipulator.h"
#include "CameraPresets.h"

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
	void CleanShaders() const;
	
	// Geometry
	OGLObject m_sphereGPU = {};
	OGLObject m_nodeGPU = {};
	GLuint  instanceVBO = 0;
	GLuint edgeEBO = 0;

	void InitGeometry();
	void CleanGeometry();

	// Textures
    GLuint m_SamplerID = 0;
	GLuint m_nodeTextureID = 0;
	GLuint m_sphereTextureID = 0;

	void InitTextures();
	void CleanTextures() const;

	// ImGui
	void InitImGuiSettings();

	template <typename EnumType, size_t N>
	bool ShowEnumDropdown(const char* label, const char* (&names)[N], EnumType& currentValue);

	ImGuiIO& io = ImGui::GetIO();
	float windowWidth;
	float windowHeight;

	// Model
	void InitSimulation();

	SimulationManager simulationManager;

	// Performance
	unsigned int fps = 0;
	unsigned int frames;
	float timeSinceLastSec = 0.0f;
	uint32_t numberOfThreads = 1;

	// Current simulation info
	int currentNumberOfParticles;

	// Light settings
	void ShowLightTypeSettings();
	void ShowLightParameterSettings();
	glm::vec4 lightPos = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
	float lightConstantAttenuation = 0.0;
	float lightLinearAttenuation = 1.0;
	float lightQuadraticAttenuation = 0.0;

	// Display settings
	const glm::vec3 DEFAULT_CAMERA_POSITION = glm::vec3(0.0f, 5.0f, 10.0f);
	const glm::vec3 DEFAULT_CAMERA_TARGET   = glm::vec3(0.0f, 0.0f,  0.0f);
	const glm::vec3 DEFAULT_CAMERA_WORLDUP  = glm::vec3(0.0f, 1.0f,  0.0f);
	float cameraDistance = 1.0f;
	float scrollZoomSpeed = 1.0f;
	void SetCameraPresetView(CameraPresets direction);
	void ShowCameraSettings();

	bool showOctree = false;
	float scaleFactor = 0.005f;
	bool isForceColor = true;
	float minForceColor = 0.0f;
	float maxForceColor = 100.0f;

	// Calculation settings
	float simulationSpeed = 1.0f;
	float threshold_simulationSpeed = 1.0f;
	float theta = 0.5f;
	float threshold_lowerTheta = 0.4f;
	float threshold_higherTheta = 1.0f;
	float epsilon = 1.0f;
	float minWorldBound = -10000;
	float maxWorldBound =  10000;

	// Load/save settings
	char saveSettingsFileName[128] = "";
	int selectedSettingsFileIndex = -1;
	std::vector<std::string> availableSettingsFiles;
	void CollectSettingsFiles();
	void SaveSettingsUI();
	void LoadSettingsUI();

	// Load/save particles
	char saveParticlesFileName[128] = "";
	int selectedParticlesFileIndex = -1;
	std::vector<std::string> availableParticlesFiles;
	void CollectParticlesFiles();
	void SaveParticlesUI();
	void LoadParticlesUI();

	// Spawn particle
	void ShowSpawnParticleSettings();
	glm::vec3 spawnParticle_position = glm::vec3(0.0f);
	float spawnParticle_mass = 1.0f;
	glm::vec3 spawnParticle_velocity = glm::vec3(0.0f);
	float spawnParticle_size = 1.0f;
	glm::vec3 spawnParticle_color = glm::vec3(1.0f);
	bool spawnParticle_movable = true;
	bool spawnParticle_show = false;

	// Spawn group
	int groupNumberOfParticles = 0;
	void ShowSpawnGoupPositionSettings(PositionType positionType);
	PositionType groupPositionType = static_cast<PositionType>(0);
	glm::vec3 groupCubeMin = glm::vec3(0.0f);
	glm::vec3 groupCubeMax = glm::vec3(0.0f);
	glm::vec3 groupSphereCenter = glm::vec3(0.0f);
	float groupSphereRadiusMin = 0.0f;
	float groupSphereRadiusMax = 0.0f;
	void ShowSpawnGoupVelocitySettings(VelocityType velocityType);
	VelocityType groupVelocityType = static_cast<VelocityType>(0);
	float groupVelocityScale = 1.0f;
	float groupCenterMass = 1.0f;
	glm::vec3 groupVelocityRandomMin = glm::vec3(0.0f);
	glm::vec3 groupVelocityRandomMax = glm::vec3(0.0f);

	// New simulation settings
	int numberOfParticles = 10000;
	int threshold_numberOfParticles = 50000;
	PresetType presetType = static_cast<PresetType>(0);
	PositionType positionType = static_cast<PositionType>(0);
	VelocityType velocityType = static_cast<VelocityType>(0);

	// Message
	void UpdateMessage(std::string newMessage, glm::vec3 newMessageColor);
	void ShowMessage(float r, float g, float b);
	glm::vec3 messageColor = glm::vec3(0);
	float messageTime = 0;
	std::string message = "";
};

